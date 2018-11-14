#include "Alpaca.hpp"
#include "AlpacaHead.hpp"
#include "AlpacaAction.hpp"

namespace Game {
namespace Alpaca {
// ActionManager class
ActionManager::ActionManager()
    : m_actions(), m_last_action(ActionManager::ActionType::NONE), m_remain(),
      m_reset_time(0.0f), m_remain_to_reset(0.0f) {
  ;
}

ActionManager::~ActionManager() {
  ;
}

void ActionManager::initWithJson(const rapidjson::Value::Object &obj) {
  assert(obj.HasMember("actions"));
  assert(obj.HasMember("resettime"));
  assert(obj["actions"].IsArray());
  assert(obj["resettime"].IsFloat());

  // read actions
  auto actions = obj["actions"].GetArray();
  assert(actions.Size() <= NUM_ACTIONS);

  StackAllocator &allocator = Application::Get().getAllocator();
  ActionFactory &factory = ActionFactory::getGlobal();
  ActionFactory::Type factoryfunc = nullptr;
  Action *new_action = nullptr;
  const char *action_type = nullptr;
  Uint32 action_type_id = 0;

  for (Uint32 i = 0; i < actions.Size(); ++i) {
    assert(actions[i].IsObject());
    auto action = actions[i].GetObject();

    assert(action.HasMember("type"));
    assert(action["type"].IsString());
    action_type = action["type"].GetString();
    action_type_id = ::Base::CompileTimeHash::runtime_hash(action_type, strlen(action_type));

    factoryfunc = factory.getFunction(action_type_id);
    new_action = factoryfunc(action, allocator);
    m_actions[i] = new_action;
  }

  m_reset_time = obj["resettime"].GetFloat();
}

void ActionManager::update() {
  auto &t = Time::Get();
  for (Uint32 i = 0; i < NUM_ACTIONS; ++i) {
    if (m_remain[i] > 0.0) {
      m_remain[i] -= t.getDeltatime();
      m_remain[i] = m_remain[i] < 0.0f ? 0.0f : m_remain[i];
    }
    if (m_actions[i])
      m_actions[i]->update();
  }

  if (!doingAnyAction()) {
    m_remain_to_reset -= t.getDeltatime();
    if (m_remain_to_reset < 0.0f && m_last_action != ActionType::NONE) {
      m_last_action = ActionType::NONE;
    }
  }
}

Action *ActionManager::getAction(ActionManager::ActionType type) {
  for (Uint32 i = 0; i < NUM_ACTIONS; ++i) {
    if (m_actions[i] && type == m_actions[i]->getType())
      return m_actions[i];
  }
  return nullptr;
}

Action *ActionManager::getAction(Uint32 idx) {
  assert(idx < NUM_ACTIONS);
  return m_actions[idx];
}

int32 ActionManager::isAbleToAction(ActionManager::ActionType type) {
  for (Uint32 i = 0; i < NUM_ACTIONS; ++i) {
    if (m_actions[i] && type == m_actions[i]->getType())
      return (m_remain[i] < 0.0f);
  }
  return false;
}

int32 ActionManager::isAbleToAction(Uint32 idx) {
  assert(idx < NUM_ACTIONS);
  return (m_remain[idx] < 0.0f);
}

int32 ActionManager::isActing(ActionManager::ActionType type) {
  for (Uint32 i = 0; i < NUM_ACTIONS; ++i) {
    if (m_actions[i] && type == m_actions[i]->getType())
      return m_actions[i]->isActing();
  }
  return false;
}

int32 ActionManager::isActing(Uint32 idx) {
  assert(idx < NUM_ACTIONS);
  if (m_actions[idx])
    return m_actions[idx]->isActing();
  return false;
}

float32 ActionManager::getRemainTime(ActionType type) {
  for(Uint32 i = 0; i < NUM_ACTIONS; ++i) {
    if(m_actions[i] && type == m_actions[i]->getType())
      return m_remain[i];
  }
  return 0.0f;
}

float32 ActionManager::getRemainTime(Uint32 idx) {
  assert(idx < NUM_ACTIONS);
  return m_remain[idx];
}

int32 ActionManager::doingAnyAction() {
  for (Uint32 i = 0; i < NUM_ACTIONS; ++i) {
    if (m_actions[i] && m_actions[i]->isActing())
      return true;
  }
  return false;
}

void ActionManager::resetRemainTimes() {
  for(Uint32 i=0; i<NUM_ACTIONS; ++i)
    m_remain[i] = 0.0f;
}

void ActionManager::doAction(ActionManager::ActionType type) {
  for (Uint32 i = 0; i < NUM_ACTIONS; ++i) {
    if (m_actions[i] && type == m_actions[i]->getType()) {
      if (m_remain[i] <= 0.0f) {
        m_actions[i]->act();
        m_remain[i] = m_actions[i]->getCooltime();
        m_last_action = type;
      }
    }
  }
}

void ActionManager::doAction(Uint32 idx) {
  assert(idx < NUM_ACTIONS);
  if (m_actions[idx] && m_remain[idx] <= 0.0f) {
    m_actions[idx]->act();
    m_remain[idx] = m_actions[idx]->getCooltime();
    m_last_action = m_actions[idx]->getType();
  }
}

void ActionManager::countResetTime() {
  m_remain_to_reset = m_reset_time;
}

ActionManager::ActionType ActionManager::getLastAction() const {
  return m_last_action;
}

void ActionManager::setAlpaca(Alpaca *alpaca) {
  for (int i = 0; i < NUM_ACTIONS; ++i) {
    if (m_actions[i])
      m_actions[i]->setAlpaca(alpaca);
  }
}

// Alpaca class
const SDL_Scancode Alpaca::keymap[2][7] =
    {
        {
            SDL_SCANCODE_W,         // up
            SDL_SCANCODE_A,         // left
            SDL_SCANCODE_S,         // down
            SDL_SCANCODE_D,         // right
            SDL_SCANCODE_1,         // heading
            SDL_SCANCODE_2,         // spit
            SDL_SCANCODE_3          // dash
        },
        {
            SDL_SCANCODE_I,         // up
            SDL_SCANCODE_J,         // left
            SDL_SCANCODE_K,         // down
            SDL_SCANCODE_L,         // right
            SDL_SCANCODE_8,         // heading
            SDL_SCANCODE_9,         // spit
            SDL_SCANCODE_0          // dash
        }
    };

Alpaca::Alpaca()
    : Component(), m_head(nullptr), m_head_object(),
      m_animator(nullptr), m_rigidbody(nullptr), m_ground(nullptr),
      m_animations(), m_speed(0.0f), m_jump_power(0.0f), m_keymap(Keymap::NONE),
      m_action_manager() {
  ;
}

Alpaca::~Alpaca() {
  ;
}

void Alpaca::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("idleanim"));
  assert(obj.HasMember("walkinganim"));
  assert(obj.HasMember("head"));
  assert(obj.HasMember("speed"));
  assert(obj.HasMember("jumppower"));
  assert(obj.HasMember("action_manager"));
  assert(obj["idleanim"].IsString());
  assert(obj["walkinganim"].IsString());
  assert(obj["head"].IsObject());
  assert(obj["speed"].IsFloat());
  assert(obj["jumppower"].IsFloat());
  assert(obj["action_manager"].IsObject());

  const char *str_idle = obj["idleanim"].GetString();
  const char *str_walking = obj["walkinganim"].GetString();
  m_animations[0] = CompileTimeHash::runtime_hash(str_idle, strlen(str_idle));
  m_animations[1] = CompileTimeHash::runtime_hash(str_walking, strlen(str_walking));

  m_speed = obj["speed"].GetFloat();
  m_jump_power = obj["jumppower"].GetFloat();

  Scene *scene = Application::Get().getScene();
  GameObject *head = scene->createGameObject(obj["head"].GetObject());
  head->setID((getGameObject()->getID()) + "head"_hash);
  m_head = head->getComponent<Head>();
  m_head->setAlpaca(this);

  m_action_manager.initWithJson(obj["action_manager"].GetObject());
  m_action_manager.setAlpaca(this);
}

void Alpaca::start() {
  auto *gameobject = getGameObject();
  m_animator = gameobject->getComponent<AnimatedSprite>();
  m_rigidbody = gameobject->getComponent<Rigidbody>();
  m_hit_gauge = gameobject->getComponent<HitGauge>();

  Component::start();
}

void Alpaca::update() {
  Component::update();
  controlledMove();
  checkActions();
  m_action_manager.update();
  m_head_object.update();
}

void Alpaca::release() {
  m_head_object.release();
}

void Alpaca::onTriggerStay(Collider *other) {
  if (other->getTag() == "ground"_hash) {
    m_ground = other->getGameObject()->getComponent<Ground>();
    setGrounded(true);
  }
}

void Alpaca::onTriggerExit(Collider *other) {
  if (other->getTag() == "ground"_hash) {
    m_ground = nullptr;
    setGrounded(false);
  }
}

Head *Alpaca::getHeadObject() {
  return m_head;
}

float32 Alpaca::getSpeed() const {
  return m_speed;
}

float32 Alpaca::getJumpPower() const {
  return m_jump_power;
}

Alpaca::Keymap Alpaca::getKeymap() const {
  return m_keymap;
}

int32 Alpaca::isMoving() const {
  return m_flags.getFlag(13);
}

int32 Alpaca::isGrounded() const {
  return m_flags.getFlag(14);
}

Rigidbody *Alpaca::getRigidbody() {
  return m_rigidbody;
}

HitGauge *Alpaca::getHitGauge() {
  return m_hit_gauge;
}

ActionManager &Alpaca::getActionManager() {
  return m_action_manager;
}

void Alpaca::setHeadObject(Head *head) {
  m_head = head;
}

void Alpaca::setSpeed(float32 speed) {
  m_speed = speed;
}

void Alpaca::setKeymap(Alpaca::Keymap keymap) {
  m_keymap = keymap;
}

void Alpaca::controlledMove() {
  auto &input = SDL::Input::Get();
  auto &t = Time::Get();
  const SDL_Scancode *keys = keymap[(int32) m_keymap];
  glm::vec3 delta = {0.0f, 0.0f, 0.0f};

  int32 hasMoved = isMoving();
  setMoving(false);
  if (!m_action_manager.doingAnyAction()) {
    const glm::vec2 &scale = getScale();
    if (input.isKeyPressed(keys[(int32) Key::UP])
        && isGrounded()
        && (m_rigidbody->getVelocity().y) <= 0.0f) {
      m_rigidbody->addForce(0.0f, m_jump_power, 0.0f);
    }
    if (input.isKeyPressed(keys[(int32) Key::DOWN]) && isGrounded()
        && m_ground) {
      m_ground->pass(this->getGameObject());
      setGrounded(false);
    }
    if (input.isKeyDown(keys[(int32) Key::LEFT])) {
      setScale(abs(scale.x), scale.y);
      delta.x -= m_speed;
      setMoving(true);
    }
    if (input.isKeyDown(keys[(int32) Key::RIGHT])) {
      setScale(-abs(scale.x), scale.y);
      delta.x += m_speed;
      setMoving(true);
    }
  }

  if (isMoving() != hasMoved) {
    auto &animations = Application::Get().getAnimationStorage();
    if (isMoving())
      m_animator->setAnimation(animations[m_animations[1]]);
    else
      m_animator->setAnimation(animations[m_animations[0]]);
  }

  delta *= t.getDeltatime();
  move(delta);
}

void Alpaca::checkActions() {
  auto &input = SDL::Input::Get();
  const SDL_Scancode *keys = keymap[(int32) m_keymap];
  if (input.isKeyDown(keys[(int32) Key::ACTION1])) {
    m_action_manager.doAction(0);
  }
  if (input.isKeyDown(keys[(int32) Key::ACTION2])) {
    m_action_manager.doAction(1);
  }
  if (input.isKeyDown(keys[(int32) Key::ACTION3])) {
    m_action_manager.doAction(2);
  }
}

void Alpaca::setMoving(int32 val) {
  m_flags.setFlag(13, val);
}

void Alpaca::setGrounded(int32 val) {
  m_flags.setFlag(14, val);
}
}
}