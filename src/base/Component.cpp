#include "Component.hpp"
#include "GameObject.hpp"

namespace Base {
// Component class
Component *Component::Factory(const ::rapidjson::Value::Object &obj,
                              ::Base::StackAllocator &allocator,
                              GameObject *gameobject) {
  fprintf(stderr, "Component is abstract class\n");
  return nullptr;
}

Component::Component()
    : m_gameobject(nullptr), m_flags() {
  ;
}

Component::Component(const Component &other)
    : m_gameobject(nullptr), m_flags() {
  m_flags.setFlag(0, other.m_flags.getFlag(0));
}

Component::~Component() {
  ;
}

Component &Component::operator=(const Component &other) {
  assert(this != &other);
  this->m_flags.setFlag(0, other.m_flags.getFlag(0));
}

void Component::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  assert(obj.HasMember("available"));
  assert(obj["available"].IsBool());

  m_flags.setFlag(0, obj["available"].GetBool());
}

void Component::start() {
  m_flags.setFlag(1, true);
}

void Component::onColliderEnter(Collider *other) {
  ;
}

void Component::onColliderStay(Collider *other) {
  ;
}

void Component::onColliderExit(Collider *other) {
  ;
}

void Component::onTriggerEnter(Collider *other) {
  ;
}

void Component::onTriggerStay(Collider *other) {
  ;
}

void Component::onTriggerExit(Collider *other) {
  ;
}

GameObject *Component::getGameObject() {
  return m_gameobject;
}

const GameObject *Component::getGameObject() const {
  return m_gameobject;
}

int32 Component::isAvailable() const {
  return m_flags.getFlag(0);
}

int32 Component::isStarted() const {
  return m_flags.getFlag(1);
}

void Component::setGameObject(GameObject *gameobject) {
  m_gameobject = gameobject;
}

void Component::setAvailable(int32 val) {
  m_flags.setFlag(0, val);
}

Uint32 Component::getTag() const {
  assert(m_gameobject);
  return m_gameobject->getTag();
}

const glm::vec3 &Component::getLocalPosition() const {
  assert(m_gameobject);
  return m_gameobject->getLocalPosition();
}

void Component::getWorldPosition(glm::vec3 *vec) const {
  assert(m_gameobject);
  m_gameobject->getWorldPosition(vec);
}

const GameObject *Component::getParent() const {
  assert(m_gameobject);
  return m_gameobject->getParent();
}

const glm::vec2 &Component::getScale() const {
  assert(m_gameobject);
  return m_gameobject->getScale();
}

float32 Component::getRotation() const {
  assert(m_gameobject);
  return m_gameobject->getRotation();
}

const glm::mat3x3 &Component::getLocalModel() const {
  assert(m_gameobject);
  return m_gameobject->getLocalModel();
}

void Component::getModel(glm::mat3x3 *mat) const {
  assert(m_gameobject);
  m_gameobject->getModel(mat);
}

void Component::setTag(Uint32 tag) {
  assert(m_gameobject);
  m_gameobject->setTag(tag);
}

void Component::setLocalPosition(const glm::vec3 &position) {
  assert(m_gameobject);
  m_gameobject->setLocalPosition(position);
}

void Component::setLocalPosition(float32 x, float32 y) {
  assert(m_gameobject);
  m_gameobject->setLocalPosition(x, y);
}

void Component::setLocalPosition(float32 x, float32 y, float32 z) {
  assert(m_gameobject);
  m_gameobject->setLocalPosition(x, y, z);
}

void Component::setWorldPosition(const glm::vec3 &position) {
  assert(m_gameobject);
  m_gameobject->setWorldPosition(position);
}

void Component::setWorldPosition(float32 x, float32 y) {
  assert(m_gameobject);
  m_gameobject->setWorldPosition(x, y);
}

void Component::setWorldPosition(float32 x, float32 y, float32 z) {
  assert(m_gameobject);
  m_gameobject->setWorldPosition(x, y, z);
}

void Component::move(const glm::vec3 &delta) {
  assert(m_gameobject);
  m_gameobject->move(delta);
}

void Component::move(float32 x, float32 y) {
  assert(m_gameobject);
  m_gameobject->move(x, y);
}

void Component::setScale(const glm::vec2 &val) {
  assert(m_gameobject);
  m_gameobject->setScale(val);
}

void Component::setScale(float32 x, float32 y) {
  assert(m_gameobject);
  m_gameobject->setScale(x, y);
}

void Component::scale(const glm::vec2 &val) {
  assert(m_gameobject);
  m_gameobject->scale(val);
}

void Component::scale(float32 x, float32 y) {
  assert(m_gameobject);
  m_gameobject->scale(x, y);
}

void Component::scale(float32 x) {
  assert(m_gameobject);
  m_gameobject->scale(x);
}

void Component::setRotation(float32 val) {
  assert(m_gameobject);
  m_gameobject->setRotation(val);
}

void Component::rotate(float32 delta) {
  assert(m_gameobject);
  m_gameobject->rotate(delta);
}

void Component::setParent(const GameObject *parent) {
  assert(m_gameobject);
  m_gameobject->setParent(parent);
}
}