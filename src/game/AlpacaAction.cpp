#include "AlpacaAction.hpp"
#include "AlpacaHead.hpp"
#include "AlpacaSpit.hpp"
#include "GameManager.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>

namespace Game {
namespace Alpaca {
// Action class
Action::Action()
    : m_cooltime(0.0f), m_alpaca(nullptr) {
  ;
}

Action::Action(const Action &other)
    : m_cooltime(other.m_cooltime), m_alpaca(nullptr) {
  ;
}

Action::~Action() {
  ;
}

Action &Action::operator=(const Action &other) {
  assert(this != &other);
  this->m_cooltime = other.m_cooltime;
  return (*this);
}

void Action::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  assert(obj.HasMember("cooltime"));
  assert(obj["cooltime"].IsFloat());

  m_cooltime = obj["cooltime"].GetFloat();
}

void Action::setAlpaca(Alpaca *alpaca) {
  m_alpaca = alpaca;
}

Alpaca *Action::getAlpaca() {
  return m_alpaca;
}

float32 Action::getCooltime() const {
  return m_cooltime;
}

// Heading class
Heading::Heading()
    : Action(), m_time(0.0f), m_length(0.0f), m_elapsed_time(0.0f),
      m_heading_target(), m_hooking_target(), m_hooking_start(), m_flags() {
  ;
}

Heading::Heading(const Heading &other)
    : Action(other), m_time(other.m_time), m_length(other.m_length), m_elapsed_time(0.0f),
      m_heading_target(), m_hooking_target(), m_hooking_start(), m_flags() {

}

Heading::~Heading() {
  ;
}

Heading &Heading::operator=(const Heading &other) {
  assert(this != &other);
  Action::operator=(other);
  this->m_time = other.m_time;
  this->m_length = other.m_length;
  return (*this);
}

void Heading::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Action::initWithJson(obj, allocator);
  assert(obj.HasMember("time"));
  assert(obj.HasMember("length"));
  assert(obj["time"].IsFloat());
  assert(obj["length"].IsFloat());

  m_time = obj["time"].GetFloat();
  m_length = obj["length"].GetFloat();
}

void Heading::update() {
  // heading
  if (isHeading()) {
    auto &t = Time::Get();
    m_elapsed_time += t.getDeltatime();

    float32 a = (-4.0f / (m_time * m_time)) * m_elapsed_time * (m_elapsed_time - m_time);

    // heading
    Alpaca *alpaca = getAlpaca();
    Head *head = alpaca->getHeadObject();
    const glm::vec3 &neck_position = head->getNeckPos();
    glm::vec3 head_position = glm::mix(neck_position, m_heading_target, a);
    head->setHeadPos(head_position);

    // hooking
    if (isHeadHooking()) {
      glm::vec3 alpaca_position = glm::mix(m_hooking_target, m_hooking_start, a);
      alpaca->setWorldPosition(alpaca_position);
    }

    // end heading
    if (m_elapsed_time > m_time) {
      if (isHeadHooking()) {
        alpaca->getRigidbody()->setGravityActive(true);
        SetHooking(false);
      }
      SetHeading(false);
      head->resetPosition();
      head->getGameObject()->getComponent<CircleCollider>()->setAvailable(false);
      alpaca->getActionManager().countResetTime();
    }
  }
}

void Heading::act() {
  Head *head = getAlpaca()->getHeadObject();
  m_heading_target = head->getNeckPos();
  m_heading_target.x -= m_length;
  head->setRotation(Math::Constant::PIf / 2.0f);
  head->getGameObject()->getComponent<CircleCollider>()->setAvailable(true);
  m_elapsed_time = 0.0f;
  SetHeading(true);
}

Action::Type Heading::getType() const {
  return Action::Type::HEADING;
}

int32 Heading::isActing() const {
  return isHeading();
}

void Heading::stopHeading() {
  if (isHeading() && m_elapsed_time < m_time / 2.0f) {
    m_elapsed_time = m_time - m_elapsed_time;
  }
}

void Heading::hooking() {
  if (isHeading()) {
    Alpaca *alpaca = getAlpaca();
    Head *head = alpaca->getHeadObject();

    glm::mat3x3 model(1.0f);
    model = glm::rotate(model, alpaca->getRotation());
    model = glm::scale(model, alpaca->getScale());
    head->getWorldPosition(&m_hooking_target);
    m_hooking_target -= model * (head->getNeckPos());

    m_hooking_start = m_hooking_target;
    glm::vec3 heading_length(m_length, 0.0f, 1.0f);
    m_hooking_start += model * heading_length;

    alpaca->getRigidbody()->setGravityActive(false);

    stopHeading();
    SetHooking(true);
  }
}

int32 Heading::isHeading() const {
  return m_flags.getFlag(0);
}

int32 Heading::isHeadHooking() const {
  return m_flags.getFlag(1);
}

void Heading::SetHeading(int32 val) {
  m_flags.setFlag(0, val);
}

void Heading::SetHooking(int32 val) {
  m_flags.setFlag(1, val);
}

// Dash class
Dash::Dash()
    : Action(), m_force(0.0f) {
  ;
}

Dash::Dash(const Dash &other)
    : Action(other), m_force(other.m_force) {
  ;
}

Dash::~Dash() {
  ;
}

Dash &Dash::operator=(const Dash &other) {
  assert(this != &other);
  Action::operator=(other);
  this->m_force = other.m_force;
  return (*this);
}

void Dash::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Action::initWithJson(obj, allocator);

  assert(obj.HasMember("force"));
  assert(obj["force"].IsFloat());

  m_force = obj["force"].GetFloat();
}

void Dash::update() {
  ;
}

void Dash::act() {
  Alpaca *alpaca = getAlpaca();
  auto &actions = alpaca->getActionManager();
  auto *heading = (Heading*)(actions.getAction(Action::Type::HEADING));
  assert(heading);

  if (heading->isActing()) {
    heading->hooking();
  } else {
    auto *alpaca_rigidbody = alpaca->getGameObject()->getComponent<Rigidbody>();
    if ((alpaca->getScale().x) < 0.0f) {
      alpaca_rigidbody->addForce(m_force, 0.0f, 0.0f);
    } else {
      alpaca_rigidbody->addForce(-m_force, 0.0f, 0.0f);
    }
  }
  actions.countResetTime();
}

Action::Type Dash::getType() const {
  return Action::Type::DASH;
}

int32 Dash::isActing() const {
  return false;
}

// Spitting class
Spitting::Spitting()
    : Action(), m_circular_spits(0), m_max_spits(0),
      m_spit_objects(nullptr), m_spits(nullptr) {
  ;
}

Spitting::~Spitting() {
  ;
}

void Spitting::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Action::initWithJson(obj, allocator);

  assert(obj.HasMember("circularspits"));
  assert(obj.HasMember("maxspits"));
  assert(obj.HasMember("spit"));
  assert(obj["circularspits"].IsUint());
  assert(obj["maxspits"].IsUint());
  assert(obj["spit"].IsObject());

  m_circular_spits = obj["circularspits"].GetUint();
  m_max_spits = obj["maxspits"].GetUint();
  auto spitjsonobj = obj["spit"].GetObject();

  m_spit_objects = allocator.alloc<GameObject*>(m_max_spits);
  m_spits = allocator.alloc<Spit*>(m_max_spits);
  memset(m_spit_objects, 0, sizeof(GameObject *) * m_max_spits);
  memset(m_spits, 0, sizeof(Spit *) * m_max_spits);

  Scene *scene = Application::Get().getScene();
  for (Uint32 i = 0; i < m_max_spits; ++i) {
    m_spit_objects[i] = scene->createGameObject(spitjsonobj);
    m_spits[i] = m_spit_objects[i]->getComponent<Spit>();
    m_spit_objects[i]->setAvailable(false);
  }
}

void Spitting::update() {
  ;
}

void Spitting::act() {
  Alpaca *alpaca = getAlpaca();
  Head *alpacahead = alpaca->getHeadObject();
  GameObject *headobject = alpacahead->getGameObject();
  auto &actions = alpaca->getActionManager();

  glm::vec3 pos(0.0f);
  headobject->getWorldPosition(&pos);
  Spit *newspit = nullptr;

  // circular
  if (Action::Type::DASH == actions.getLastAction()) {
    float32 angle = 0.0f;
    float32 delta = 2.0f * Math::Constant::PIf / (float32) m_circular_spits;
    glm::vec3 dir(0.0f);
    for (Uint32 i = 0; i < m_circular_spits; ++i) {
      dir.x = cosf(angle);
      dir.y = sinf(angle);

      newspit = GetSpit();
      newspit->setWorldPosition(pos);
      newspit->shoot(dir, alpaca);

      angle += delta;
    }
  }
    // default
  else {
    newspit = GetSpit();
    newspit->setWorldPosition(pos);

    auto alpaca_num = (Uint32)alpaca->getKeymap();
    alpaca_num = alpaca_num ? 0 : 1;
    GameManager *game_manager = GameManager::GetGlobal();
    glm::vec3 opponent_position(0.0f);
    game_manager->getAlpaca(alpaca_num)->getWorldPosition(&opponent_position);
    newspit->shoot(opponent_position - pos, alpaca);
  }
  actions.countResetTime();
}

Action::Type Spitting::getType() const {
  return Action::Type::SPIT;
}

int32 Spitting::isActing() const {
  return false;
}

Spit *Spitting::GetSpit() {
  for (Uint32 i = 0; i < m_max_spits; ++i) {
    if (!(m_spit_objects[i]->isAvailable())) {
      m_spit_objects[i]->setAvailable(true);
      return m_spits[i];
    }
  }
  fprintf(stderr, "there is no remain spit\n");
  exit(-1);
}
}
}