#include "GameObject.hpp"
#include "Application.hpp"
#include "Scene.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Base {
// GameObject class
GameObject *GameObject::Factory(const rapidjson::Value::Object &obj, StackAllocator &allocator,
                                ObjectStorage *storage, Uint32 id) {
  assert(storage);
  auto *result = new(allocator.alloc<GameObject>()) GameObject();
  assert(result);
  result->setID(id);
  storage->add(result);
  result->initWithJson(obj, allocator);
  return result;
}

GameObject::GameObject()
    : m_id(0), m_component_count(0), m_rotation(0.0f), m_components(nullptr),
      m_parent(nullptr), m_position(), m_scale(), m_model(), m_flags() {
  setAvailable(true);
}

GameObject::GameObject(Uint32 id, int32 isStatic)
    : m_id(id), m_component_count(0), m_rotation(0.0f), m_components(nullptr),
      m_parent(nullptr), m_position(), m_scale(), m_model(), m_flags() {
  setAvailable(true);
  m_flags.setFlag(3, isStatic);
}

GameObject::GameObject(Uint32 id, const GameObject *parent, int32 isStatic)
    : m_id(id), m_component_count(0), m_rotation(0.0f), m_components(nullptr),
      m_parent(parent), m_position(), m_scale(), m_model(), m_flags() {
  setAvailable(true);
  m_flags.setFlag(3, isStatic);
}

GameObject::GameObject(const GameObject &other)
    : m_id(0), m_component_count(0), m_rotation(other.m_rotation), m_components(nullptr),
      m_parent(other.m_parent), m_position(other.m_position), m_scale(other.m_scale),
      m_model(), m_flags() {
  m_flags.setFlag(1, other.m_flags.getFlag(1));
  m_flags.setFlag(3, other.m_flags.getFlag(3));
}

GameObject::~GameObject() {
  ;
}

GameObject &GameObject::operator=(const GameObject &other) {
  assert(this != &other);
  this->m_position = other.m_position;
  this->m_parent = other.m_parent;
  this->m_scale = other.m_scale;
  this->m_rotation = other.m_rotation;
  this->m_flags.setFlag(1, other.m_flags.getFlag(1));
  this->m_flags.setFlag(3, other.m_flags.getFlag(3));
  return (*this);
}

void GameObject::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  assert(obj.HasMember("tag"));
  assert(obj.HasMember("available"));
  assert(obj.HasMember("static"));
  assert(obj.HasMember("position"));
  assert(obj.HasMember("scale"));
  assert(obj.HasMember("rotation"));
  assert(obj.HasMember("components"));
  assert(obj["tag"].IsString());
  assert(obj["available"].IsBool());
  assert(obj["static"].IsBool());
  assert(obj["position"].IsObject());
  assert(obj["scale"].IsObject());
  assert(obj["rotation"].IsFloat());
  assert(obj["components"].IsArray());

  // read elements
  const char *tag = obj["tag"].GetString();
  m_tag = CompileTimeHash::runtime_hash(tag, strlen(tag));
  setAvailable(obj["available"].GetBool());
  m_flags.setFlag(3, obj["static"].GetBool());
  JsonParseMethods::ReadVector(obj["position"].GetObject(), &m_position);
  JsonParseMethods::ReadVector2(obj["scale"].GetObject(), &m_scale);
  m_rotation = obj["rotation"].GetFloat();

  // read components
  auto component_list = obj["components"].GetArray();
  m_component_count = component_list.Size();

  auto &factory = ComponentFactory::getGlobal();
  m_components = allocator.alloc<Component *>(m_component_count);
  Component::FactoryFunc *factoryfunc = nullptr;
  Component *new_component = nullptr;
  const char *type = nullptr;

  // read parent
  if (obj.HasMember("parent")) {
    assert(obj["parent"].IsObject());
    Application &app = Application::Get();
    Scene *scene = app.getScene();
    assert(scene);
    auto parent_data = obj["parent"].GetObject();
    assert(parent_data.HasMember("storage"));
    assert(parent_data.HasMember("id"));
    assert(parent_data["storage"].IsString());
    assert(parent_data["id"].IsString());

    const char *str_storage_id = parent_data["storage"].GetString();
    const char *str_id = parent_data["id"].GetString();
    Uint32 storage_id = CompileTimeHash::runtime_hash(str_storage_id, strlen(str_storage_id));
    Uint32 id = CompileTimeHash::runtime_hash(str_id, strlen(str_id));
    m_parent = scene->getObject(storage_id, id);
    assert(m_parent);
  }

  for (int i = 0; i < m_component_count; ++i) {
    assert(component_list[i].IsObject());
    auto component_obj = component_list[i].GetObject();

    assert(component_obj.HasMember("type"));
    assert(component_obj["type"].IsString());

    type = component_obj["type"].GetString();
    factoryfunc = factory.getFunction(CompileTimeHash::runtime_hash(type, strlen(type)));
    assert(factoryfunc);
    new_component = factoryfunc(component_obj, allocator, this);
    assert(new_component);

    new_component->setGameObject(this);
    m_components[i] = new_component;
  }

}

void GameObject::start() {
  // set model matrix
  m_model = glm::mat3x3(1.0f);
  glm::vec2 local_position = getLocalPosition();
  m_model = glm::translate(m_model, local_position);
  m_model = glm::rotate(m_model, getRotation());
  m_model = glm::scale(m_model, getScale());

  for (int i = 0; i < m_component_count; ++i) {
    m_components[i]->start();
  }

  m_flags.setFlag(2, true);
}

void GameObject::update() {
  if (!isStarted())
    start();

  if (!isStatic()) {
    // update model matrix
    m_model = glm::mat3x3(1.0f);
    glm::vec2 local_position = getLocalPosition();
    m_model = glm::translate(m_model, local_position);
    m_model = glm::rotate(m_model, getRotation());
    m_model = glm::scale(m_model, getScale());
  }

  for (int i = 0; i < m_component_count; ++i) {
    m_components[i]->update();
  }
}

void GameObject::release() {
  for (int i = 0; i < m_component_count; ++i) {
    m_components[i]->release();
    m_components[i]->~Component();
    m_components[i] = nullptr;
  }
}

GameObject *GameObject::addChild(GameObject *child) const {
  child->setParent(this);
  return child;
}

void GameObject::onColliderEnter(Collider *other) {
  for (int i = 0; i < m_component_count; ++i) {
    m_components[i]->onColliderEnter(other);
  }
}

void GameObject::onColliderStay(Collider *other) {
  for (int i = 0; i < m_component_count; ++i) {
    m_components[i]->onColliderStay(other);
  }
}

void GameObject::onColliderExit(Collider *other) {
  for (int i = 0; i < m_component_count; ++i) {
    m_components[i]->onColliderExit(other);
  }
}

void GameObject::onTriggerEnter(Collider *other) {
  for (int i = 0; i < m_component_count; ++i) {
    m_components[i]->onTriggerEnter(other);
  }
}

void GameObject::onTriggerStay(Collider *other) {
  for (int i = 0; i < m_component_count; ++i) {
    m_components[i]->onTriggerStay(other);
  }
}

void GameObject::onTriggerExit(Collider *other) {
  for (int i = 0; i < m_component_count; ++i) {
    m_components[i]->onTriggerExit(other);
  }
}

Uint32 GameObject::getID() const {
  return m_id;
}

Uint32 GameObject::getTag() const {
  return m_tag;
}

const glm::vec3 &GameObject::getLocalPosition() const {
  return m_position;
}

void GameObject::getWorldPosition(glm::vec3 *vec) const {
  if (!m_parent)
    *vec = m_position;
  else {
    glm::mat3x3 model(1.0f);
    getModel(&model);
    (*vec).x = (*vec).y = 0.0f;
    (*vec).z = 1.0f;
    *vec = model * (*vec);
  }
}

const glm::vec2 &GameObject::getScale() const {
  return m_scale;
}

float32 GameObject::getRotation() const {
  return m_rotation;
}

const glm::mat3x3 &GameObject::getLocalModel() const {
  return m_model;
}

void GameObject::getModel(glm::mat3x3 *mat) const {
  if (!m_parent)
    *mat = m_model;
  else {
    const GameObject *parent = m_parent;
    *mat = (parent->getLocalModel()) * (this->getLocalModel());
    while (nullptr != (parent = parent->getParent())) {
      *mat = (parent->getLocalModel()) * (*mat);
    }
  }
}

Uint32 GameObject::getComponentCount() const {
  return m_component_count;
}

const GameObject *GameObject::getParent() const {
  return m_parent;
}

int32 GameObject::isDeleted() const {
  return m_flags.getFlag(0);
}

int32 GameObject::isAvailable() const {
  return m_flags.getFlag(1);
}

int32 GameObject::isStarted() const {
  return m_flags.getFlag(2);
}

int32 GameObject::isStatic() const {
  return m_flags.getFlag(3);
}

void GameObject::setID(Uint32 id) {
  if (false == m_flags.getFlag(4)) {
    m_id = id;
    m_flags.setFlag(4, true);
  }
}

void GameObject::setTag(Uint32 tag) {
  m_tag = tag;
}

void GameObject::setLocalPosition(const glm::vec3 &position) {
  m_position = position;
}

void GameObject::setLocalPosition(float32 x, float32 y) {
  m_position.x = x;
  m_position.y = y;
}

void GameObject::setLocalPosition(float32 x, float32 y, float32 z) {
  m_position.x = x;
  m_position.y = y;
  m_position.z = z;
}

void GameObject::setWorldPosition(const glm::vec3 &position) {
  if (!m_parent)
    m_position = position;
  else {
    glm::mat3x3 model(1.0f);
    m_parent->getModel(&model);
    model = glm::inverse(model);
    m_position = model * position;
  }
}

void GameObject::setWorldPosition(float32 x, float32 y) {
  if (!m_parent) {
    m_position.x = x;
    m_position.y = y;
  } else {
    glm::vec3 pos(x, y, m_position.z);
    glm::mat3x3 model(1.0f);
    m_parent->getModel(&model);
    model = glm::inverse(model);
    m_position = model * pos;
  }
}

void GameObject::setWorldPosition(float32 x, float32 y, float32 z) {
  if (!m_parent) {
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
  } else {
    glm::vec3 pos(x, y, z);
    glm::mat3x3 model(1.0f);
    m_parent->getModel(&model);
    model = glm::inverse(model);
    m_position = model * pos;
  }
}

void GameObject::move(const glm::vec3 &delta) {
  m_position += delta;
}

void GameObject::move(float32 x, float32 y) {
  m_position.x += x;
  m_position.y += y;
}

void GameObject::setScale(const glm::vec2 &val) {
  m_scale = val;
}

void GameObject::setScale(float32 x, float32 y) {
  m_scale.x = x;
  m_scale.y = y;
}

void GameObject::scale(const glm::vec2 &val) {
  m_scale.x *= val.x;
  m_scale.y *= val.y;
}

void GameObject::scale(float32 x, float32 y) {
  m_scale.x *= x;
  m_scale.y *= y;
}

void GameObject::scale(float32 x) {
  m_scale *= x;
}

void GameObject::setRotation(float32 val) {
  m_rotation = val;
}

void GameObject::rotate(float32 delta) {
  m_rotation += delta;
}

void GameObject::setParent(const GameObject *parent) {
  m_parent = parent;
}

void GameObject::del() {
  m_flags.setFlag(0, true);
}

void GameObject::setAvailable(int32 value) {
  m_flags.setFlag(1, value);
}

// Object Storage
ObjectStorage::ObjectStorage()
    : m_id(0), m_order(0), m_len(0), m_gameobjects(nullptr) {
  ;
}

ObjectStorage::~ObjectStorage() {
  clear();
}

void ObjectStorage::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  assert(obj.HasMember("name"));
  assert(obj.HasMember("order"));
  assert(obj.HasMember("size"));
  assert(obj["name"].IsString());
  assert(obj["order"].IsUint());
  assert(obj["size"].IsUint());

  const char *name = obj["name"].GetString();
  StringID id(name);
  m_id = (Uint32) id;

  m_order = obj["order"].GetUint();

  m_len = obj["size"].GetUint();
  m_gameobjects = allocator.alloc<Type>(m_len);
  memset(m_gameobjects, 0, sizeof(Type) * m_len);
}

GameObject *ObjectStorage::add(GameObject *gameobject) {
  for (Uint32 i = 0; i < m_len; ++i) {
    if (!m_gameobjects[i]) {
      m_gameobjects[i] = gameobject;
      return gameobject;
    }
  }
  fprintf(stderr, "there is no space in ObjectStorage\n");
  exit(-1);
}

GameObject *ObjectStorage::get(Uint32 hash) {
  for (Uint32 i = 0; i < m_len; ++i) {
    if (m_gameobjects[i] && hash == m_gameobjects[i]->getID()) {
      return m_gameobjects[i];
    }
  }
  return nullptr;
}

void ObjectStorage::updateObjects() {
  for (Uint32 i = 0; i < m_len; ++i) {
    if (m_gameobjects[i] && m_gameobjects[i]->isAvailable()) {
      m_gameobjects[i]->update();
    }
  }
}

void ObjectStorage::checkAndDelete() {
  for (Uint32 i = 0; i < m_len; ++i) {
    if (m_gameobjects[i] && m_gameobjects[i]->isDeleted()) {
      m_gameobjects[i]->release();
      m_gameobjects[i]->~GameObject();
      m_gameobjects[i] = nullptr;
    }
  }
}

Uint32 ObjectStorage::getID() const {
  return m_id;
}

Uint32 ObjectStorage::getOrder() const {
  return m_order;
}

void ObjectStorage::clear() {
  for (Uint32 i = 0; i < m_len; ++i) {
    if (m_gameobjects[i]) {
      m_gameobjects[i]->release();
      m_gameobjects[i]->~GameObject();
      m_gameobjects[i] = nullptr;
    }
  }
}
}

