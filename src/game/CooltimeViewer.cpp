#include "CooltimeViewer.h"
#include "GameManager.hpp"

namespace Game {
// CooltimeViewer class
CooltimeViewer::CooltimeViewer()
  :m_alpaca_index(0), m_action_index(0),
  m_sprite(nullptr), m_default_uv(), m_default_position(), m_default_scale(),
  m_action(nullptr), m_alpaca(nullptr) {
  ;
}

CooltimeViewer::~CooltimeViewer() {
  ;
}

void CooltimeViewer::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("alpacaindex"));
  assert(obj.HasMember("actionindex"));
  assert(obj["alpacaindex"].IsUint());
  assert(obj["actionindex"].IsUint());

  m_alpaca_index = obj["alpacaindex"].GetUint();
  m_action_index = obj["actionindex"].GetUint();
}

void CooltimeViewer::start() {
  m_sprite = getGameObject()->getComponent<Sprite>();
  assert(m_sprite);
  m_default_uv = m_sprite->getUV();
  m_default_position = getLocalPosition();
  m_default_scale = getScale();

  m_alpaca = GameManager::GetGlobal()->getAlpaca(m_alpaca_index);
  assert(m_alpaca);
  auto &action_manager = m_alpaca->getActionManager();
  m_action = action_manager.getAction(m_action_index);

  Component::start();
}

void CooltimeViewer::update() {
  Component::update();

  auto &action_manager = m_alpaca->getActionManager();
  float32 percentage = (1.0f - (action_manager.getRemainTime(m_action_index))/(m_action->getCooltime()));

  Math::Rect uv = m_default_uv;
  uv.h *= percentage;
  m_sprite->setUV(uv);

  glm::vec3 position = m_default_position;
  glm::vec2 scale = m_default_scale;
  scale.y *= percentage;
  position.y -= m_default_scale.y/2.0f;
  position.y += scale.y/2.0f;
  setLocalPosition(position);
  setScale(scale);
}

void CooltimeViewer::release() {
  ;
}

}