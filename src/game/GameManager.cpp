#include "GameManager.hpp"

namespace Game {
// GameManager class
GameManager::GameManager()
    : Component(), m_alpacas(),
      m_life_count(0), m_remain_life(),
      m_alpaca_spawn_position(), m_alpaca_spawn_scale(),
      m_effects() {
  ;
}

GameManager::~GameManager() {
  ;
}

void GameManager::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("lifecount"));
  assert(obj.HasMember("alpaca0"));
  assert(obj.HasMember("alpaca1"));
  assert(obj.HasMember("effect"));
  assert(obj["lifecount"].IsUint());
  assert(obj["alpaca0"].IsObject());
  assert(obj["alpaca1"].IsObject());
  assert(obj["effect"].IsObject());

  m_life_count = obj["lifecount"].GetUint();
  m_remain_life[0] = m_remain_life[1] = m_life_count;
  auto alpaca0json = obj["alpaca0"].GetObject();
  auto alpaca1json = obj["alpaca1"].GetObject();
  auto effect_json = obj["effect"].GetObject();

  Scene *scene = Application::Get().getScene();
  assert(scene);

  GameObject *newalpaca = nullptr;
  newalpaca = scene->createGameObject(alpaca0json);
  assert(newalpaca);
  m_alpacas[0] = newalpaca->getComponent<Alpaca::Alpaca>();
  assert(m_alpacas[0]);
  m_alpacas[0]->setKeymap(Alpaca::Alpaca::Keymap::MAP1);
  m_alpacas[0]->getWorldPosition(&m_alpaca_spawn_position[0]);
  m_alpaca_spawn_scale[0] = m_alpacas[0]->getScale();

  newalpaca = scene->createGameObject(alpaca1json);
  assert(newalpaca);
  m_alpacas[1] = newalpaca->getComponent<Alpaca::Alpaca>();
  assert(m_alpacas[1]);
  m_alpacas[1]->setKeymap(Alpaca::Alpaca::Keymap::MAP2);
  m_alpacas[1]->getWorldPosition(&m_alpaca_spawn_position[1]);
  m_alpaca_spawn_scale[1] = m_alpacas[1]->getScale();

  GameObject *new_object = nullptr;
  for(Uint32 i=0; i<2; ++i) {
    new_object = scene->createGameObject(effect_json);
    m_effects[i] = new_object->getComponent<OutsidedEffect>();
  }

  assert(!global);
  global = this;
}

void GameManager::start() {
}

void GameManager::update() {
  auto *scene = Application::Get().getScene();
  auto *camera = scene->getCamera("main"_hash);
  const glm::vec3 &ltn = camera->getLeftTopNear();
  const glm::vec3 &rbf = camera->getRightBottomFar();

  glm::vec3 position(0.0f);
  for(Uint32 i=0; i<2; ++i) {
    m_alpacas[i]->getWorldPosition(&position);
    if(position.x < ltn.x || position.x > rbf.x
    || position.y < rbf.y || position.y > ltn.y) {
      --m_remain_life[i];
      if(!m_remain_life[i]) {
        gameOver(i);
      }

      m_effects[i]->setWorldPosition(position);
      m_effects[i]->generate(-(m_alpacas[i]->getRigidbody()->getVelocity()));

      m_alpacas[i]->setWorldPosition(m_alpaca_spawn_position[i]);
      m_alpacas[i]->setScale(m_alpaca_spawn_scale[i]);
      m_alpacas[i]->getRigidbody()->setVelocity(0.0f, 0.0f, 0.0f);
      m_alpacas[i]->getHitGauge()->resetGauge();
    }
  }
}

void GameManager::release() {
  ;
}

void GameManager::gameOver(Uint32 idx) {
  printf("player %d win!", idx ? 0 : 1);
  exit(1);
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