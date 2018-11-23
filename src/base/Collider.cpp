#include "Collider.hpp"
#include "Physics.hpp"
#include "Application.hpp"
#include "Scene.hpp"

namespace Base {
// Collider class
Component *Collider::Factory(const ::rapidjson::Value::Object &obj,
                             ::Base::StackAllocator &allocator,
                             GameObject *gameobject) {
  fprintf(stderr, "Collider is abstract class\n");
  return nullptr;
}

Collider::Collider()
    : Component() {
  ;
}

Collider::Collider(const Collider &other)
    : Component(other) {
  ;
}

Collider::~Collider() {
  ;
}

Collider &Collider::operator=(const Collider &other) {
  assert(this != &other);
  Component::operator=(other);
  return (*this);
}

void Collider::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("isTrigger"));
  assert(obj["isTrigger"].IsBool());
  setTrigger(obj["isTrigger"].GetBool());
}

void Collider::start() {
  m_rigidbody = getGameObject()->getComponent<Rigidbody>();

  Scene *scene = Application::Get().getScene();
  assert(scene);
  scene->getPhysics().add(this);

  Component::start();
}

void Collider::update() {
  Component::update();
}

void Collider::release() {
  ;
}

Rigidbody *Collider::getRigidbody() {
  return m_rigidbody;
}

const Rigidbody *Collider::getRigidbody() const {
  return m_rigidbody;
}

int32_t Collider::isTrigger() const {
  return m_flags.getFlag(2);
}

void Collider::setTrigger(int32_t val) {
  m_flags.setFlag(2, val);
}

// CircleCollider class
CircleCollider::CircleCollider()
    : Collider(), m_radius(0.0f) {
  ;
}

CircleCollider::CircleCollider(const CircleCollider &other)
    : Collider(other), m_radius(other.m_radius) {
  ;
}

CircleCollider::~CircleCollider() {
  ;
}

CircleCollider &CircleCollider::operator=(const CircleCollider &other) {
  assert(this != &other);
  Collider::operator=(other);
  this->m_radius = other.m_radius;
  return (*this);
}

void CircleCollider::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Collider::initWithJson(obj, allocator);

  assert(obj.HasMember("radius"));
  assert(obj["radius"].IsFloat());

  m_radius = obj["radius"].GetFloat();
}

int32_t CircleCollider::isCollideWith(const Collider *other) const {
  switch (other->getType()) {
    case Collider::Type::CIRCLE:
      return CollideCheckFunctions::isCollide(this, (CircleCollider *) other);
      break;

    case Collider::Type::BOX:
      return CollideCheckFunctions::isCollide((BoxCollider *) other, this);
      break;

    default:
      fprintf(stderr, "unknown collider\n");
      exit(-1);
      break;
  }
}

Collider::Type CircleCollider::getType() const {
  return Collider::Type::CIRCLE;
}

float32 CircleCollider::getRadius() const {
  return m_radius;
}

void CircleCollider::setRadius(float32 r) {
  m_radius = r;
}

// Box Collider class
BoxCollider::BoxCollider()
    : Collider(), m_box() {
  ;
}

BoxCollider::BoxCollider(const BoxCollider &other)
    : Collider(other), m_box(other.m_box) {
  ;
}

BoxCollider::~BoxCollider() {
  ;
}

BoxCollider &BoxCollider::operator=(const BoxCollider &other) {
  assert(this != &other);
  Collider::operator=(other);
  this->m_box = other.m_box;
  return (*this);
}

void BoxCollider::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Collider::initWithJson(obj, allocator);

  assert(obj.HasMember("box"));
  assert(obj["box"].IsObject());

  JsonParseMethods::ReadRect(obj["box"].GetObject(), &m_box);
}

int32_t BoxCollider::isCollideWith(const Collider *other) const {
  switch (other->getType()) {
    case Collider::Type::CIRCLE:
      return CollideCheckFunctions::isCollide(this, (CircleCollider *) other);
      break;
    case Collider::Type::BOX:
      return CollideCheckFunctions::isCollide(this, (BoxCollider *) other);
      break;
    default:
      fprintf(stderr, "unknown collider\n");
      exit(-1);
      break;
  }
  return false;
}

Collider::Type BoxCollider::getType() const {
  return Collider::Type::BOX;
}

const Math::Rect &BoxCollider::getBox() const {
  return m_box;
}

void BoxCollider::setBox(const Math::Rect &box) {
  m_box = box;
}


// CollideCheckFunctions
namespace CollideCheckFunctions {
int32_t isCollide(const CircleCollider *circle0, const CircleCollider *circle1) {
  assert(circle0);
  assert(circle1);

  glm::vec3 pos0, pos1;
  circle0->getWorldPosition(&pos0);
  circle1->getWorldPosition(&pos1);
  const glm::vec3 delta = pos0 - pos1;
  float32 radiussum = (circle0->getRadius()) + (circle1->getRadius());

  if ((delta.x * delta.x + delta.y * delta.y) < radiussum * radiussum) {
    return true;
  } else {
    return false;
  }
}

int32_t isCollide(const BoxCollider *box, const CircleCollider *circle) {
  assert(box);
  assert(circle);

  glm::vec3 boxpos, circlepos;
  box->getWorldPosition(&boxpos);
  circle->getWorldPosition(&circlepos);

  Math::Rect rect = box->getBox();
  float32 radius = circle->getRadius();

  rect.x = boxpos.x + rect.x - rect.w / 2.0f;
  rect.y = boxpos.y + rect.y - rect.h / 2.0f;

  int8_t in[4] = {0,};  // left right bottom top

  // check in l, r, b, t
  in[0] = circlepos.x > rect.x;
  in[1] = circlepos.x < rect.x + rect.w;
  in[2] = circlepos.y > rect.y;
  in[3] = circlepos.y < rect.y + rect.h;

  if (in[0] && in[1] && in[2] && in[3]) {
    return true;
  } else if (in[2] && in[3]) {
    return ((circlepos.x + radius > rect.x) && (circlepos.x - radius < rect.x + rect.w));
  } else if (in[0] && in[1]) {
    return ((circlepos.y + radius > rect.y) && (circlepos.y - radius < rect.y + rect.h));
  }

  glm::vec3 delta = {0.0f, 0.0f, 0.0f}, point = {0.0f, 0.0f, circlepos.z};
  point.x = in[0] ? (rect.x + rect.w) : rect.x;
  point.y = in[2] ? (rect.y + rect.h) : rect.y;
  delta = point - circlepos;
  if ((delta.x * delta.x + delta.y * delta.y) < radius * radius) {
    return true;
  } else {
    return false;
  }
}

int32_t isCollide(const BoxCollider *box0, const BoxCollider *box1) {
  assert(box0);
  assert(box1);

  glm::vec3 pos0, pos1;
  box0->getWorldPosition(&pos0);
  box1->getWorldPosition(&pos1);
  Math::Rect r0 = box0->getBox();
  Math::Rect r1 = box1->getBox();

  r0.x = pos0.x + r0.x - r0.w / 2.0f;
  r0.y = pos0.y + r0.y - r0.h / 2.0f;

  r1.x = pos1.x + r1.x - r1.w / 2.0f;
  r1.y = pos1.y + r1.y - r1.h / 2.0f;

  int32_t inX = (r0.x > r1.x && r0.x < r1.x + r1.w) || (r1.x > r0.x && r1.x < r0.x + r0.w);
  int32_t inY = (r0.y > r1.y && r0.y < r1.y + r1.h) || (r1.y > r0.y && r1.y < r0.y + r0.h);
  if (inX && inY) {
    return true;
  } else {
    return false;
  }
}
}
}