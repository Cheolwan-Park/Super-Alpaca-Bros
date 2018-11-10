#ifndef GameGameManager_hpp
#define GameGameManager_hpp

#include "base.hpp"
#include "Alpaca.hpp"

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

  Alpaca::Alpaca *getAlpaca(Uint32 idx);

 private:
  static GameManager *global;

 public:
  static GameManager *GetGlobal();

 private:
  Alpaca::Alpaca *m_alpacas[2];
  float32 m_respawn_time;
  Uint32 m_life_count;
  Uint32 m_remain_life[2];
};
}

#endif