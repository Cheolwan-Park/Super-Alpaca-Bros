#include "GameManager.hpp"

namespace Game {
// GameManager class
GameManager::GameManager()
    : Component(), m_alpacas(),
      m_respawn_time(0.0f), m_life_count(0), m_remain_life() {
  ;
}

GameManager::~GameManager() {
  ;
}

void GameManager::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("respawntime"));
  assert(obj.HasMember("lifecount"));
  assert(obj.HasMember("alpaca0"));
  assert(obj.HasMember("alpaca1"));
  assert(obj["respawntime"].IsFloat());
  assert(obj["lifecount"].IsUint());
  assert(obj["alpaca0"].IsObject());
  assert(obj["alpaca1"].IsObject());

  m_respawn_time = obj["respawntime"].GetFloat();
  m_life_count = obj["lifecount"].GetUint();
  m_remain_life[0] = m_remain_life[1] = m_life_count;
  auto alpaca0json = obj["alpaca0"].GetObject();
  auto alpaca1json = obj["alpaca1"].GetObject();

  Scene *scene = Application::Get().getScene();
  assert(scene);

  GameObject *newalpaca = nullptr;
  newalpaca = scene->createGameObject(alpaca0json);
  assert(newalpaca);
  m_alpacas[0] = newalpaca->getComponent<Alpaca::Alpaca>();
  assert(m_alpacas[0]);
  m_alpacas[0]->setKeymap(Alpaca::Alpaca::Keymap::MAP1);

  newalpaca = scene->createGameObject(alpaca1json);
  assert(newalpaca);
  m_alpacas[1] = newalpaca->getComponent<Alpaca::Alpaca>();
  assert(m_alpacas[1]);
  m_alpacas[1]->setKeymap(Alpaca::Alpaca::Keymap::MAP2);

  assert(!global);
  global = this;
}

void GameManager::start() {
}

void GameManager::update() {
  ;
}

void GameManager::release() {
  ;
}

Alpaca::Alpaca *GameManager::getAlpaca(Uint32 idx) {
  assert(idx < 2);
  return m_alpacas[idx];
}

GameManager *GameManager::global = nullptr;
GameManager *GameManager::GetGlobal() {
  return global;
}
}