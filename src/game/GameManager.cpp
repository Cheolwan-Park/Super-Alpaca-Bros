#include "GameManager.hpp"
#include "AlpacaHead.hpp"
#include "AlpacaAction.hpp"
#include "Banner.h"

namespace Game {
// GameManager class
GameManager::GameManager()
    : Component(), m_alpacas(),
      m_life_count(0), m_remain_life(),
      m_respawn_time(0.0f), m_respawn_remain_time(),
      m_alpaca_spawn_position(), m_alpaca_spawn_scale(),
      m_effects(), m_pause_ui(nullptr) {
  ;
}

GameManager::~GameManager() {
  ;
}

void GameManager::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("lifecount"));
  assert(obj.HasMember("respawntime"));
  assert(obj.HasMember("alpaca"));
  assert(obj.HasMember("effect"));
  assert(obj["respawntime"].IsFloat());
  assert(obj["lifecount"].IsUint());
  assert(obj["alpaca"].IsArray());
  assert(obj["effect"].IsArray());

  m_life_count = obj["lifecount"].GetUint();
  m_remain_life[0] = m_remain_life[1] = m_life_count;
  m_respawn_time = obj["respawntime"].GetFloat();
  m_respawn_remain_time[0] = m_respawn_remain_time[1] = 0.0f;
  auto alpaca_list = obj["alpaca"].GetArray();
  auto effect_list = obj["effect"].GetArray();
  assert(2 == alpaca_list.Size());
  assert(2 == effect_list.Size());

  Scene *scene = Application::Get().getScene();
  assert(scene);

  GameObject *newalpaca = nullptr;
  for(Uint32 i=0; i<2; ++i) {
    newalpaca = scene->createGameObject(alpaca_list[i].GetObject());
    assert(newalpaca);
    m_alpacas[i] = newalpaca->getComponent<Alpaca::Alpaca>();
    assert(m_alpacas[i]);
    m_alpacas[i]->setKeymap((Alpaca::Alpaca::Keymap)i);
    m_alpacas[i]->getWorldPosition(&m_alpaca_spawn_position[i]);
    m_alpaca_spawn_scale[i] = m_alpacas[i]->getScale();

  }

  GameObject *new_object = nullptr;
  for(Uint32 i=0; i<2; ++i) {
    new_object = scene->createGameObject(effect_list[i].GetObject());
    m_effects[i] = new_object->getComponent<OutsidedEffect>();
  }

  assert(!global);
  global = this;
}

void GameManager::start() {
  auto *scene = Application::Get().getScene();
  auto *pause_object = scene->getObject("ui"_hash, "UI_Pause"_hash);
  assert(pause_object);
  m_pause_ui = pause_object->getComponent<BannerMover>();
}

void GameManager::update() {
  Component::update();

  auto *scene = Application::Get().getScene();
  auto *camera = scene->getCamera("main"_hash);
  const glm::vec3 &ltn = camera->getLeftTopNear();
  const glm::vec3 &rbf = camera->getRightBottomFar();

  glm::vec3 position(0.0f);
  for(Uint32 i=0; i<2; ++i) {
    // check respawn
    if(m_respawn_remain_time[i] > 0.0f) {
      Time &t = Time::Get();
      m_respawn_remain_time[i] -= t.getDeltatime();
      if(m_respawn_remain_time[i] < 0.0f) {
        m_respawn_remain_time[i] = 0.0f;
        m_alpacas[i]->getGameObject()->setAvailable(true);
        m_alpacas[i]->setWorldPosition(m_alpaca_spawn_position[i]);
        m_alpacas[i]->setScale(m_alpaca_spawn_scale[i]);
      }
    }

    // check alpaca out of screen (KO)
    m_alpacas[i]->getWorldPosition(&position);
    int32 is_out_of_screen = position.x < ltn.x || position.x > rbf.x
                          || position.y < rbf.y || position.y > ltn.y;
    if((m_alpacas[i]->getGameObject()->isAvailable())
    && is_out_of_screen) {
      --m_remain_life[i];

      m_effects[i]->setWorldPosition(position);
      if(position.x < ltn.x || position.x > rbf.x) {
        m_effects[i]->generate(-position);
      }
      else {
        m_effects[i]->generate(-(m_alpacas[i]->getRigidbody()->getVelocity()));
      }

      m_alpacas[i]->getRigidbody()->setVelocity(0.0f, 0.0f, 0.0f);
      m_alpacas[i]->getHeadObject()->resetPosition();
      m_alpacas[i]->getHitGauge()->resetGauge();
      auto &action_manager = m_alpacas[i]->getActionManager();
      action_manager.resetRemainTimes();
      auto *heading = (Alpaca::Heading*)action_manager.getAction(Alpaca::ActionManager::ActionType::HEADING);
      heading->stopHooking();
      m_alpacas[i]->setWorldPosition(m_alpaca_spawn_position[i]);
      m_alpacas[i]->getGameObject()->setAvailable(false);

      // sound effect
      auto &mixer = SDL::Mixer::Get();
      auto &chunks = Application::Get().getChunkStorage();
      if(!m_remain_life[i]) {
        // sound effect
        mixer.playChunk(chunks["end_explosion.wav"_hash], 2);
        gameOver(i);
        break;
      } else {
        // sound effect
        mixer.playChunk(chunks["explosion.wav"_hash], 3);
      }

      m_respawn_remain_time[i] = m_respawn_time;
    }
  }


  // check pause
  SDL::Input &input = SDL::Input::Get();
  if(input.isKeyPressed(SDL_SCANCODE_ESCAPE)
  && !(m_pause_ui->isMoving())) {
    if(m_pause_ui->isShowed()) {
      resume();
      m_pause_ui->hide();
    } else {
      pause();
      m_pause_ui->show();
    }
  }
}

void GameManager::release() {
  ;
}

void GameManager::gameOver(Uint32 idx) {
  auto *winner_banner = WinnerBanner::GetGlobal();
  assert(winner_banner);
  winner_banner->show(idx ? 0 : 1);
  pause();
}

void GameManager::pause() {
  for(Uint32 i=0; i<2; ++i) {
    m_alpacas[i]->getRigidbody()->setAvailable(false);
    m_alpacas[i]->setAvailable(false);
  }
}

void GameManager::resume() {
  for(Uint32 i=0; i<2; ++i) {
    m_alpacas[i]->getRigidbody()->setAvailable(true);
    m_alpacas[i]->setAvailable(true);
  }
}

void GameManager::restart() {
  for(Uint32 i=0; i<2; ++i) {
    m_alpacas[i]->getGameObject()->setAvailable(true);
    m_alpacas[i]->setAvailable(true);
    m_alpacas[i]->getRigidbody()->setAvailable(true);
    m_alpacas[i]->getRigidbody()->setVelocity(0.0f, 0.0f, 0.0f);
    m_alpacas[i]->getHeadObject()->resetPosition();
    m_alpacas[i]->getHitGauge()->resetGauge();
    auto &action_manager = m_alpacas[i]->getActionManager();
    auto *heading = (Alpaca::Heading*)action_manager.getAction(Alpaca::ActionManager::ActionType::HEADING);
    heading->stopHooking();
    action_manager.resetRemainTimes();
    m_alpacas[i]->setWorldPosition(m_alpaca_spawn_position[i]);
    m_alpacas[i]->setScale(m_alpaca_spawn_scale[i]);
    m_remain_life[i] = m_life_count;
  }
}

Alpaca::Alpaca *GameManager::getAlpaca(Uint32 idx) {
  assert(idx < 2);
  return m_alpacas[idx];
}

Uint32 GameManager::getRemainLife(Uint32 idx) {
  assert(idx < 2);
  return m_remain_life[idx];
}

GameManager *GameManager::global = nullptr;
GameManager *GameManager::GetGlobal() {
  return global;
}


// LifeViewer class
LifeViewer::LifeViewer()
  :m_game_manager(nullptr), m_life_count(0), m_life_hearts(nullptr) {

}

LifeViewer::~LifeViewer() {
  ;
}

void LifeViewer::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("alpacaindex"));
  assert(obj.HasMember("lifecount"));
  assert(obj.HasMember("heart"));
  assert(obj["alpacaindex"].IsUint());
  assert(obj["lifecount"].IsUint());
  assert(obj["heart"].IsObject());

  m_alpaca_index = obj["alpacaindex"].GetUint();
  m_life_count = obj["lifecount"].GetUint();
  m_life_hearts = allocator.alloc<GameObject*>(m_life_count);
  memset(m_life_hearts, 0, sizeof(GameObject*)*m_life_count);
  auto heart_json_object = obj["heart"].GetObject();

  auto *scene = Application::Get().getScene();
  glm::vec3 position(0.0f, 0.0f, 1.0f);
  for(int32 i=0; i<m_life_count; ++i) {
    m_life_hearts[i] = scene->createGameObject(heart_json_object);
    const glm::vec2 &scale = m_life_hearts[i]->getScale();
    position.x = (i - (int32)m_life_count/2)*scale.x;
    position.x -= m_life_count&1 ? 0.0f : scale.x/2.0f;
    m_life_hearts[i]->setLocalPosition(position);
    m_life_hearts[i]->setParent(getGameObject());
  }
}

void LifeViewer::start() {
  m_game_manager = GameManager::GetGlobal();
}

void LifeViewer::update() {
  Component::update();
  Uint32 i=0, remain = m_game_manager->getRemainLife(m_alpaca_index);
  for(i=0; i<remain; ++i) {
    m_life_hearts[i]->setAvailable(true);
  }
  for(;i<m_life_count; ++i) {
    m_life_hearts[i]->setAvailable(false);
  }
}

void LifeViewer::release() {
  ;
}



}