#include "Banner.h"
#include "GameManager.hpp"

namespace Game {
// BannerMover class
BannerMover::BannerMover()
    :Component(), m_elapsed_time(0.0f), m_duration(0.0f),
     m_hide_position(), m_show_position(), m_target_position(), m_start_position() {
  ;
}

BannerMover::~BannerMover() {
  ;
}

void BannerMover::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("duration"));
  assert(obj.HasMember("hideposition"));
  assert(obj.HasMember("showposition"));
  assert(obj["duration"].IsFloat());
  assert(obj["hideposition"].IsObject());
  assert(obj["showposition"].IsObject());

  m_duration = obj["duration"].GetFloat();
  JsonParseMethods::ReadVector(obj["hideposition"].GetObject(), &m_hide_position);
  JsonParseMethods::ReadVector(obj["showposition"].GetObject(), &m_show_position);
}

void BannerMover::start() {
  setWorldPosition(m_hide_position);
  Component::start();
}

void BannerMover::update() {
  Component::update();

  if(isMoving()) {
    Time &t = Time::Get();
    m_elapsed_time += t.getDeltatime();
    glm::vec3 position(0.0f);
    if(m_elapsed_time < m_duration)
      position = glm::mix(m_start_position, m_target_position, m_elapsed_time / m_duration);
    else {
      setMoving(false);
      position = m_target_position;
    }
    setWorldPosition(position);
  }
}

void BannerMover::release() {
  ;
}

void BannerMover::show() {
  m_start_position = m_hide_position;
  m_target_position = m_show_position;
  m_elapsed_time = 0.0f;
  setShowed(true);
  setMoving(true);
}

void BannerMover::hide() {
  m_start_position = m_show_position;
  m_target_position = m_hide_position;
  m_elapsed_time = 0.0f;
  setShowed(false);
  setMoving(true);
}

float32 BannerMover::getDuration() const {
  return m_duration;
}

int32_t BannerMover::isMoving() const {
  return m_flags.getFlag(2);
}

int32_t BannerMover::isShowed() const {
  return m_flags.getFlag(3);
}

void BannerMover::setMoving(int32_t val) {
  m_flags.setFlag(2, val);
}

void BannerMover::setShowed(int32_t val) {
  m_flags.setFlag(3, val);
}


// WinnerBanner class
WinnerBanner::WinnerBanner()
  :Component(), m_elapsed_time(0.0f), m_wait_time(0.0f), m_duration(0.0f),
  m_rotate(0.0f), m_max_scale(0.0f),
  m_default_rotation(0.0f), m_default_scale(0.0f),
  m_rotation(0.0f), m_scale(0.0f),
  m_win_banner_uvs(), m_mover(nullptr), m_sprite(nullptr) {
  ;
}

WinnerBanner::~WinnerBanner() {
  ;
}

void WinnerBanner::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("waittime"));
  assert(obj.HasMember("duration"));
  assert(obj.HasMember("rotate"));
  assert(obj.HasMember("scale"));
  assert(obj.HasMember("banneruv"));
  assert(obj["waittime"].IsFloat());
  assert(obj["duration"].IsFloat());
  assert(obj["rotate"].IsFloat());
  assert(obj["scale"].IsFloat());
  assert(obj["banneruv"].IsArray());

  m_wait_time = obj["waittime"].GetFloat();
  m_duration = obj["duration"].GetFloat();
  m_rotate = obj["rotate"].GetFloat();
  m_max_scale = obj["scale"].GetFloat();

  auto load_list = obj["banneruv"].GetArray();
  assert(2 == load_list.Size());
  assert(load_list[0].IsObject());
  assert(load_list[1].IsObject());

  JsonParseMethods::ReadIRect(load_list[0].GetObject(), &m_win_banner_uvs[0]);
  JsonParseMethods::ReadIRect(load_list[1].GetObject(), &m_win_banner_uvs[1]);

  global = this;
}

void WinnerBanner::start() {
  m_mover = getGameObject()->getComponent<BannerMover>();
  m_sprite = getGameObject()->getComponent<Sprite>();
  m_default_rotation = getRotation();
  m_default_scale = getScale();
  Component::start();
}

void WinnerBanner::update() {
  Component::update();

  Time &t = Time::Get();
  if(m_mover->isShowed() && !(m_mover->isMoving())) {
    // banner effect
    m_elapsed_time += t.getDeltatime();
    float32 theta = 2.0f * Math::Constant::PIf * m_elapsed_time/m_duration;
    float32 sin = sinf(theta);

    m_scale = m_default_scale;
    m_scale *= (1.0f + sin * m_max_scale);

    m_rotation = m_default_rotation;
    m_rotation += sin*m_rotate;

    setScale(m_scale);
    setRotation(m_rotation);

    // check input
    if(m_elapsed_time > m_wait_time) {
      SDL::Input &input = SDL::Input::Get();
      if (input.isAnyKeyPressed() || input.isButtonDown(SDL_BUTTON_LEFT)) {
        auto *game_manager = GameManager::GetGlobal();
        game_manager->restart();
        hide();
        m_elapsed_time = 0.0f;
      }
    }
  } else if(m_mover->isMoving()) {
    m_elapsed_time += t.getDeltatime();
    float32 percentage = m_elapsed_time/(m_mover->getDuration());
    glm::vec2 scale = glm::mix(m_scale, m_default_scale, percentage);
    float32 rotation = m_rotation + (m_default_rotation - m_rotation)*percentage;
    setScale(scale);
    setRotation(rotation);
  }
}

void WinnerBanner::release() {
  ;
}

void WinnerBanner::show(uint32_t winner) {
  assert(winner < 2);
  m_sprite->setUV(m_win_banner_uvs[winner]);
  setRotation(m_default_rotation);
  setScale(m_default_scale);
  m_elapsed_time = 0.0f;
  m_mover->show();
}

void WinnerBanner::hide() {
  m_mover->hide();
}

WinnerBanner *WinnerBanner::global = nullptr;
WinnerBanner *WinnerBanner::GetGlobal() {
  return global;
}

}