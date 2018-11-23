#ifndef GameGameManager_hpp
#define GameGameManager_hpp

#include "base.hpp"
#include "Alpaca.hpp"
#include "OutsidedEffect.h"

using namespace Base;

namespace Game {
class GameManager : public Component {
 public:
  COMPONENT(GameManager);

  GameManager();

  GameManager(GameManager &other) = delete;

  ~GameManager() override;

  GameManager &operator=(const GameManager &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void gameOver(uint32_t idx);

  void pause();

  void resume();

  void restart();

  Alpaca::Alpaca *getAlpaca(uint32_t idx);

  uint32_t getRemainLife(uint32_t idx);

 private:
  static GameManager *global;

 public:
  static GameManager *GetGlobal();

 private:
  Alpaca::Alpaca *m_alpacas[2];
  uint32_t m_life_count;
  uint32_t m_remain_life[2];
  float32 m_respawn_time;
  float32 m_respawn_remain_time[2];
  glm::vec3 m_alpaca_spawn_position[2];
  glm::vec2 m_alpaca_spawn_scale[2];

  OutsidedEffect *m_effects[2];
  class BannerMover *m_pause_ui;
};

class LifeViewer : public Component {
 public:
  COMPONENT(LifeViewer);

  LifeViewer();

  LifeViewer(LifeViewer &other) = delete;

  ~LifeViewer() override;

  LifeViewer &operator=(const LifeViewer &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

 private:
  GameManager *m_game_manager;
  uint32_t m_alpaca_index;
  uint32_t m_life_count;
  GameObject **m_life_hearts;
};
}

#endif