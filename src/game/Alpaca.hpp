#ifndef GameAlpaca_hpp
#define GameAlpaca_hpp

#include "base.hpp"
#include "Ground.hpp"
#include "HitGauge.hpp"

using namespace Base;

namespace Game {
namespace Alpaca {
class Head;
class Action;
class Alpaca;

class ActionManager {
 public:
  static constexpr Uint32 NUM_ACTIONS = 4;
  enum class ActionType : Uint32 { NONE = 0, HEADING = 1, SPIT = 2, DASH = 3 };

 public:
  ActionManager();

  ActionManager(const ActionManager &other) = delete;

  ~ActionManager();

  ActionManager &operator=(const ActionManager &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj);

  void update();

  Action *getAction(ActionType type);

  Action *getAction(Uint32 idx);

  int32 isAbleToAction(ActionType type);

  int32 isAbleToAction(Uint32 idx);

  int32 isActing(ActionType type);

  int32 isActing(Uint32 idx);

  float32 getRemainTime(ActionType type);

  float32 getRemainTime(Uint32 idx);

  int32 doingAnyAction();

  void resetRemainTimes();

  void doAction(ActionType type);

  void doAction(Uint32 idx);

  void countResetTime();

  ActionType getLastAction() const;

  void setAlpaca(Alpaca *alpaca);

 private:
  FixedArray<Action *, NUM_ACTIONS> m_actions;
  ActionType m_last_action;
  float32 m_remain[NUM_ACTIONS];
  float32 m_reset_time;
  float32 m_remain_to_reset;
};

class Alpaca : public Component {
 public:
  static const SDL_Scancode keymap[2][7];
  enum class Keymap : int32 { NONE = -1, MAP1 = 0, MAP2 = 1 };
  enum class Key : int32 {
    UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3,
    ACTION1 = 4, ACTION2 = 5, ACTION3 = 6
  };

 public:
  COMPONENT(Alpaca);

  Alpaca();

  Alpaca(const Alpaca &other) = delete;

  ~Alpaca() override;

  Alpaca &operator=(const Alpaca &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void onTriggerStay(Collider *other) override;

  void onTriggerExit(Collider *other) override;

  // get
  Head *getHeadObject();

  float32 getSpeed() const;

  float32 getJumpPower() const;

  Alpaca::Keymap getKeymap() const;

  int32 isMoving() const;

  int32 isGrounded() const;

  Rigidbody *getRigidbody();

  HitGauge *getHitGauge();

  ActionManager &getActionManager();

  // set
  void setHeadObject(Head *head);

  void setSpeed(float32 speed);

  void setKeymap(Alpaca::Keymap keymap);

 private:
  void controlledMove();

  void checkActions();

  void setMoving(int32 val);

  void setGrounded(int32 val);

 private:
  Head *m_head;
  GameObject m_head_object;
  AnimatedSprite *m_animator;
  Rigidbody *m_rigidbody;
  HitGauge *m_hit_gauge;
  Ground *m_ground;
  Uint32 m_animations[2];  // 0 : idle, 1 : walking
  float32 m_speed;
  float32 m_jump_power;
  Keymap m_keymap;
  ActionManager m_action_manager;
};
}
}

#endif