#include "AlpacaSpit.hpp"
#include "Alpaca.hpp"

namespace Game {
namespace Alpaca {
Spit::Spit()
    : Component(), m_elapsed_time(0.0f), m_lifetime(0.0f),
      m_speed(0.0f), m_force(0.0f), m_gauge_up(0.0f),
      m_owner(nullptr), m_direction() {
  ;
}

Spit::Spit(const Spit &other)
    : Component(other), m_elapsed_time(0.0f), m_lifetime(other.m_lifetime),
      m_speed(other.m_speed), m_force(other.m_force), m_gauge_up(other.m_gauge_up),
      m_owner(other.m_owner), m_direction(other.m_direction) {
  ;
}

Spit::~Spit() {
  ;
}

Spit &Spit::operator=(const Spit &other) {
  assert(this != &other);
  Component::operator=(other);
  this->m_lifetime = other.m_lifetime;
  this->m_speed = other.m_speed;
  this->m_force = other.m_force;
  this->m_gauge_up = other.m_gauge_up;
  this->m_owner = other.m_owner;
  this->m_direction = other.m_direction;
  return (*this);
}

void Spit::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("lifetime"));
  assert(obj.HasMember("speed"));
  assert(obj.HasMember("force"));
  assert(obj.HasMember("gaugeup"));
  assert(obj["lifetime"].IsFloat());
  assert(obj["speed"].IsFloat());
  assert(obj["force"].IsFloat());
  assert(obj["gaugeup"].IsFloat());

  m_lifetime = obj["lifetime"].GetFloat();
  m_speed = obj["speed"].GetFloat();
  m_force = obj["force"].GetFloat();
  m_gauge_up = obj["gaugeup"].GetFloat();
}

void Spit::start() {
  Component::start();
}

void Spit::update() {
  Component::update();
  if (isShot()) {
    Time &t = Time::Get();
    move(m_direction * (float32) (m_speed * t.getDeltatime()));

    m_elapsed_time += t.getDeltatime();
    if (m_elapsed_time > m_lifetime) {
      getGameObject()->setAvailable(false);
      m_flags.setFlag(2, false);
    }
  }
}

void Spit::release() {
  ;
}

void Spit::onTriggerEnter(Collider *other) {
  if ((other->getTag()) == "player"_hash
      && (other->getGameObject()) != (m_owner->getGameObject())) {
    auto *alpaca = other->getGameObject()->getComponent<Alpaca>();
    Rigidbody *rigid = alpaca->getRigidbody();
    HitGauge *hit_gauge = alpaca->getHitGauge();

    float32 force = m_force * (hit_gauge->GetFactor());
    rigid->addForce(m_direction * force);
    hit_gauge->gaugeUp(m_gauge_up);

    getGameObject()->setAvailable(false);

    // sound effect
    auto &mixer = SDL::Mixer::Get();
    auto &chunks = Application::Get().getChunkStorage();
    mixer.playChunk(chunks["hit.wav"_hash]);
  }
}

void Spit::shoot(const glm::vec3 &direction, Alpaca *owner) {
  m_direction = glm::normalize(direction);
  m_owner = owner;
  m_elapsed_time = 0.0f;
  m_flags.setFlag(2, true);
}

int32 Spit::isShot() const {
  return m_flags.getFlag(2);
}
}
}