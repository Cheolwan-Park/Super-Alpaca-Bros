#include "Physics.hpp"
#include "Collider.hpp"
#include "Time.hpp"

namespace Base {
// Physics class
Physics::Physics()
    : m_collider_count(0), m_max_collider_count(0), m_colliders(nullptr), m_collide_map(nullptr),
      m_gravity(0.0f) {
  ;
}

Physics::~Physics() {
  ;
}

void Physics::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  assert(obj.HasMember("collider_count"));
  assert(obj.HasMember("gravity"));
  assert(obj["collider_count"].IsUint());
  assert(obj["gravity"].IsFloat());

  m_max_collider_count = obj["collider_count"].GetUint();
  m_gravity = obj["gravity"].GetFloat();

  m_colliders = allocator.alloc<Collider *>(m_max_collider_count);
  memset(m_colliders, 0, sizeof(Collider *) * m_max_collider_count);
  m_collide_map = (byte *) allocator.alloc<byte>(m_max_collider_count * m_max_collider_count);
  memset(m_collide_map, 0, sizeof(byte) * m_max_collider_count * m_max_collider_count);
}

void Physics::update() {
  Time &t = Time::Get();
  Rigidbody *rigidbody = nullptr;

  byte *collide_map = m_collide_map;
  for (Uint32 i = 0; i < m_max_collider_count; ++i) {
    if (!m_colliders[i]
        || !m_colliders[i]->isAvailable()
        || !m_colliders[i]->getGameObject()->isAvailable()) {
      continue;
    }

    // apply gravity
    rigidbody = m_colliders[i]->getRigidbody();
    if (rigidbody && rigidbody->isGravityActive()) {
      rigidbody->accelerate(0.0f, -m_gravity * t.getDeltatime(), 0.0f);
    }

    // check collide
    for (Uint32 j = i + 1; j < m_max_collider_count; ++j) {
      if (!m_colliders[j]
          || !m_colliders[j]->isAvailable()
          || !m_colliders[j]->getGameObject()->isAvailable()) {
        continue;
      }
      if (m_colliders[i]->isCollideWith(m_colliders[j])) {
        if (collide_map[j]) {
          if (m_colliders[i]->isTrigger() || m_colliders[j]->isTrigger()) {
            m_colliders[i]->getGameObject()->onTriggerStay(m_colliders[j]);
            m_colliders[j]->getGameObject()->onTriggerStay(m_colliders[i]);
          } else {
            m_colliders[i]->getGameObject()->onColliderStay(m_colliders[j]);
            m_colliders[j]->getGameObject()->onColliderStay(m_colliders[i]);
          }
        } else {
          if (m_colliders[i]->isTrigger() || m_colliders[j]->isTrigger()) {
            m_colliders[i]->getGameObject()->onTriggerEnter(m_colliders[j]);
            m_colliders[j]->getGameObject()->onTriggerEnter(m_colliders[i]);
          } else {
            m_colliders[i]->getGameObject()->onColliderEnter(m_colliders[j]);
            m_colliders[j]->getGameObject()->onColliderEnter(m_colliders[i]);
          }
          collide_map[j] = true;
        }
      } else if (collide_map[j]) {
        if (m_colliders[i]->isTrigger() || m_colliders[j]->isTrigger()) {
          m_colliders[i]->getGameObject()->onTriggerExit(m_colliders[j]);
          m_colliders[j]->getGameObject()->onTriggerExit(m_colliders[i]);
        } else {
          m_colliders[i]->getGameObject()->onColliderExit(m_colliders[j]);
          m_colliders[j]->getGameObject()->onColliderExit(m_colliders[i]);
        }
        collide_map[j] = false;
      }
    }
    collide_map += m_max_collider_count;
  }
}

void Physics::checkDeleted() {
  byte *iter_collide_map = nullptr;
  byte *self_collide_map = m_collide_map;
  for (Uint32 i = 0; i < m_max_collider_count; ++i) {
    if (m_colliders[i] && m_colliders[i]->getGameObject()->isDeleted()) {
      m_colliders[i] = nullptr;
      iter_collide_map = m_collide_map;
      for (Uint32 j = 0; j < i; ++j) {
        if (iter_collide_map[i]) {
          m_colliders[i]->getGameObject()->onColliderExit(m_colliders[j]);
          m_colliders[j]->getGameObject()->onColliderExit(m_colliders[i]);
          iter_collide_map[i] = 0;
        }
        iter_collide_map += m_max_collider_count;
      }
      for (Uint32 j = i + 1; j < m_max_collider_count; ++j) {
        if (self_collide_map[j]) {
          m_colliders[i]->getGameObject()->onColliderExit(m_colliders[j]);
          m_colliders[j]->getGameObject()->onColliderExit(m_colliders[i]);
          self_collide_map[j] = 0;
        }
      }

      --m_collider_count;
    }
    self_collide_map += m_max_collider_count;
  }
}

Collider *Physics::add(Collider *collider) {
  for (Uint32 i = 0; i < m_max_collider_count; ++i) {
    if (!m_colliders[i]) {
      m_colliders[i] = collider;
      ++m_collider_count;
      return collider;
    }
  }
  return nullptr;
}

float32 Physics::getGravity() const {
  return m_gravity;
}

void Physics::setGravity(float32 a) {
  m_gravity = a;
}

Uint32 Physics::getColliderCount() const {
  return m_collider_count;
}

Uint32 Physics::getMaxColliderCount() const {
  return m_max_collider_count;
}

// Rigidbody class
Rigidbody::Rigidbody()
    : Component(), m_mass(0.0f), m_drag(0.0f), m_velocity() {
  setSimulated(true);
}

Rigidbody::Rigidbody(const Rigidbody &other)
    : Component(other), m_mass(other.m_mass), m_drag(other.m_drag), m_velocity(other.m_velocity) {
  this->setSimulated(other.isSimulated());
}

Rigidbody::~Rigidbody() {
  ;
}

Rigidbody &Rigidbody::operator=(const Rigidbody &other) {
  assert(this != &other);
  Component::operator=(other);
  this->m_mass = other.m_mass;
  this->m_drag = other.m_drag;
  this->m_velocity = other.m_velocity;
  this->setSimulated(other.isSimulated());
  return (*this);
}

void Rigidbody::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("mass"));
  assert(obj.HasMember("drag"));
  assert(obj.HasMember("isSimulated"));
  assert(obj.HasMember("isGravityActive"));
  assert(obj["mass"].IsFloat());
  assert(obj["drag"].IsFloat());
  assert(obj["isSimulated"].IsBool());
  assert(obj["isGravityActive"].IsBool());

  m_mass = obj["mass"].GetFloat();
  m_drag = obj["drag"].GetFloat();
  setSimulated(obj["isSimulated"].GetBool());
  setGravityActive(obj["isGravityActive"].GetBool());
}

void Rigidbody::start() {
  Component::start();
}

void Rigidbody::update() {
  if (isSimulated()) {
    Time &t = Time::Get();
    addForce(m_velocity * (float32) (-m_drag * t.getDeltatime() / m_mass));
    glm::vec3 delta = m_velocity * (float32) t.getDeltatime();
    move(delta);
  }
}
void Rigidbody::release() {
  ;
}

void Rigidbody::onColliderEnter(Collider *other) {
  Rigidbody *other_rigidbody = other->getRigidbody();

  const glm::vec3 &v1 = this->getVelocity();
  const glm::vec3 &v2 = other_rigidbody->getVelocity();
  float32 m1 = this->getMass();
  float32 m2 = other_rigidbody->getMass();

  glm::vec3 u1(0.0f);
  u1 = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2);

  this->setVelocity(u1);
}

void Rigidbody::accelerate(const glm::vec3 &a) {
  m_velocity += a;
}

void Rigidbody::accelerate(float32 x, float32 y, float32 z) {
  m_velocity.x += x;
  m_velocity.y += y;
  m_velocity.z += z;
}

void Rigidbody::addForce(const glm::vec3 &f) {
  m_velocity += f / m_mass;
}

void Rigidbody::addForce(float32 x, float32 y, float32 z) {
  m_velocity.x += x / m_mass;
  m_velocity.y += y / m_mass;
  m_velocity.z += z / m_mass;
}

float32 Rigidbody::getMass() const {
  return m_mass;
}

const glm::vec3 &Rigidbody::getVelocity() const {
  return m_velocity;
}

int32 Rigidbody::isSimulated() const {
  return m_flags.getFlag(2);
}

int32 Rigidbody::isGravityActive() const {
  return m_flags.getFlag(3);
}

void Rigidbody::setMass(float32 m) {
  m_mass = m;
}

void Rigidbody::setVelocity(const glm::vec3 &v) {
  m_velocity = v;
}

void Rigidbody::setVelocity(float32 x, float32 y, float32 z) {
  m_velocity.x = x;
  m_velocity.y = y;
  m_velocity.z = z;
}

void Rigidbody::setSimulated(int32 val) {
  m_flags.setFlag(2, val);
}

void Rigidbody::setGravityActive(int32 val) {
  m_flags.setFlag(3, val);
}
}