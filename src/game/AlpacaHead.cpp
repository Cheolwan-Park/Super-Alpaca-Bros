#include "AlpacaHead.hpp"
#include "Alpaca.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>

namespace Game {
namespace Alpaca {
Head::Head()
    : Sprite(), m_alpaca(nullptr), m_force(0.0f), m_force_ratio(),
      m_necks(), m_neck_sprites(), m_head_position(), m_neck_position() {
  ;
}

Head::~Head() {
  ;
}

void Head::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Sprite::initWithJson(obj, allocator);

  assert(obj.HasMember("force"));
  assert(obj.HasMember("forceratio"));
  assert(obj.HasMember("gaugeup"));
  assert(obj.HasMember("headpos"));
  assert(obj.HasMember("neckpos"));
  assert(obj.HasMember("neck"));
  assert(obj["force"].IsFloat());
  assert(obj["forceratio"].IsObject());
  assert(obj["gaugeup"].IsFloat());
  assert(obj["headpos"].IsObject());
  assert(obj["neckpos"].IsObject());
  assert(obj["neck"].IsObject());

  m_force = obj["force"].GetFloat();
  JsonParseMethods::ReadVector2(obj["forceratio"].GetObject(), &m_force_ratio);
  m_gauge_up = obj["gaugeup"].GetFloat();
  JsonParseMethods::ReadVector(obj["headpos"].GetObject(), &m_head_position);
  JsonParseMethods::ReadVector(obj["neckpos"].GetObject(), &m_neck_position);

  auto neckobj = obj["neck"].GetObject();
  for (int i = 0; i < m_necks.getMaxSize(); ++i) {
    m_necks[i].initWithJson(neckobj, allocator);
    m_neck_sprites[i] = m_necks[i].getComponent<Sprite>();
  }
}

void Head::start() {
  getGameObject()->setLocalPosition(m_head_position);
  for (int i = 0; i < m_necks.getMaxSize(); ++i) {
    m_necks[i].start();
    m_necks[i].setLocalPosition(m_neck_position);
  }
  Sprite::start();
}

void Head::update() {
  Sprite::update();
  for (int i = 0; i < m_necks.getMaxSize(); ++i) {
    m_necks[i].update();
  }
}

void Head::release() {
  for (int i = 0; i < m_necks.getMaxSize(); ++i) {
    m_necks[i].release();
    m_necks[i].~GameObject();
  }
  Sprite::release();
}

void Head::onTriggerEnter(Collider *other) {
  if (other->getTag() == "player"_hash && other->getGameObject() != m_alpaca->getGameObject()) {
    auto *alpaca = other->getGameObject()->getComponent<Alpaca>();
    Rigidbody *rigid = alpaca->getRigidbody();
    HitGauge *hitgauge = alpaca->getHitGauge();
    float32 force = m_force * (hitgauge->GetFactor());
    if (m_alpaca->getScale().x > 0.0f) {
      rigid->addForce(-force * m_force_ratio.x, 0.0f, 0.0f);
    } else {
      rigid->addForce(force * m_force_ratio.x, 0.0f, 0.0f);
    }
    rigid->addForce(0.0f, force * m_force_ratio.y, 0.0f);

    hitgauge->gaugeUp(m_gauge_up);

    // sound effect
    auto &mixer = SDL::Mixer::Get();
    auto &chunks = Application::Get().getChunkStorage();
    mixer.playChunk(chunks["hit.wav"_hash]);
  }
}

void Head::draw() {
  for (int i = 0; i < m_necks.getMaxSize(); ++i) {
    m_neck_sprites[i]->draw();
  }
  Sprite::draw();
}

void Head::resetPosition() {
  setLocalPosition(m_head_position);
  setRotation(0.0f);
  for (int i = 0; i < m_necks.getMaxSize(); ++i)
    m_necks[i].setLocalPosition(m_neck_position);
}

void Head::setAlpaca(Alpaca *alpaca) {
  assert(alpaca);
  m_alpaca = alpaca;

  setParent(alpaca->getGameObject());
  for (int i = 0; i < m_necks.getMaxSize(); ++i) {
    m_necks[i].setParent(alpaca->getGameObject());
  }

  setTag((alpaca->getTag()) + "head"_hash);
  setTag((alpaca->getTag()) + "neck"_hash);
}

void Head::setHeadPos(const glm::vec3 &pos) {
  for (int i = 0; i < m_necks.getMaxSize(); ++i) {
    m_necks[i].setLocalPosition(glm::mix(m_neck_position, pos, (float32) i / (float32) (m_necks.getMaxSize())));
  }
  setLocalPosition(pos);
}

GameObject &Head::getNeck(int32 i) {
  return m_necks[i];
}

size_t Head::getNeckCount() const {
  return m_necks.getMaxSize();
}

float32 Head::getForce() const {
  return m_force;
}

float32 Head::getGaugeUp() const {
  return m_gauge_up;
}

const glm::vec3 &Head::getHeadPos() const {
  return m_head_position;
}

const glm::vec3 &Head::getNeckPos() const {
  return m_neck_position;
}
}
}