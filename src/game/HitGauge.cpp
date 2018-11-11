#include "HitGauge.hpp"
#include "GameManager.hpp"

namespace Game {
// HitGauge class
HitGauge::HitGauge()
    : Component(), m_gauge(0.0f), m_max_gauge(0.0f),
      m_min_factor(0.0f), m_max_factor(0.0f),
      m_animation_time(0.0f), m_remain_time(0.0f), m_start(0.0f), m_target(0.0f) {
  ;
}

HitGauge::HitGauge(const HitGauge &other)
    : Component(other), m_gauge(other.m_gauge), m_max_gauge(other.m_max_gauge),
      m_min_factor(other.m_min_factor), m_max_factor(other.m_max_factor),
      m_animation_time(other.m_animation_time), m_remain_time(0.0f),
      m_start(0.0f), m_target(0.0f) {
  ;
}

HitGauge::~HitGauge() {
  ;
}

HitGauge &HitGauge::operator=(const HitGauge &other) {
  assert(this != &other);
  Component::operator=(other);
  this->m_gauge = other.m_gauge;
  this->m_max_gauge = other.m_max_gauge;
  this->m_min_factor = other.m_min_factor;
  this->m_max_factor = other.m_max_factor;
  this->m_animation_time = other.m_animation_time;
  return (*this);
}

void HitGauge::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("maxgauge"));
  assert(obj.HasMember("minfactor"));
  assert(obj.HasMember("maxfactor"));
  assert(obj.HasMember("animationtime"));
  assert(obj["maxgauge"].IsFloat());
  assert(obj["minfactor"].IsFloat());
  assert(obj["maxfactor"].IsFloat());
  assert(obj["animationtime"].IsFloat());

  m_max_gauge = obj["maxgauge"].GetFloat();
  m_min_factor = obj["minfactor"].GetFloat();
  m_max_factor = obj["maxfactor"].GetFloat();
  m_animation_time = obj["animationtime"].GetFloat();
}

void HitGauge::start() {
  Component::start();
}

void HitGauge::update() {
  if(m_remain_time > 0.0f)
  {
    Time &t = Time::Get();
    m_gauge = m_start + (m_target - m_start)*((m_animation_time-m_remain_time)/m_animation_time);
    m_remain_time -= t.getDeltatime();
  }
  else
  {
    m_gauge = m_target;
    m_remain_time = 0.0f;
  }
}

void HitGauge::release() {
  ;
}

void HitGauge::resetGauge() {
  m_start = m_gauge;
  m_target = 0.0f;
  m_remain_time = m_animation_time;
}

void HitGauge::gaugeUp(float32 val) {
  m_start = m_gauge;
  m_target = m_target + val;
  m_target = m_target > m_max_gauge ? m_max_gauge : m_target;
  m_remain_time = m_animation_time;
}

float32 HitGauge::getGauge() const {
  return m_gauge;
}

float32 HitGauge::GetFactor() const {
  float32 percentage = m_gauge / m_max_gauge;
  return (m_min_factor + (m_max_factor - m_min_factor) * percentage * percentage);
}

// NumberViewer class
NumberViewer::NumberViewer()
    : Sprite(), m_number_sheet(nullptr) {
  ;
}

NumberViewer::NumberViewer(const NumberViewer &other)
    : Sprite(other), m_number_sheet(other.m_number_sheet) {
  ;
}

NumberViewer::~NumberViewer() {
  ;
}

NumberViewer &NumberViewer::operator=(const NumberViewer &other) {
  Sprite::operator=(other);
  this->m_number_sheet = other.m_number_sheet;
  return (*this);
}

void NumberViewer::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Sprite::initWithJson(obj, allocator);

  assert(obj.HasMember("numbersheet"));
  assert(obj["numbersheet"].IsString());

  const char *str_number_sheet = obj["numbersheet"].GetString();
  StringID number_sheet_id(str_number_sheet);
  auto &animations = Application::Get().getAnimationStorage();

  m_number_sheet = animations[(Uint32)number_sheet_id];
  const Math::Size &sprite_count = m_number_sheet->getSpritesCount();
}

void NumberViewer::start() {
  Sprite::start();
  setNumber(0);
}

void NumberViewer::update() {
  Sprite::update();
}

void NumberViewer::release() {
  Sprite::release();
}

void NumberViewer::setNumber(Uint32 n) {
  assert(n < 10);

  const Math::IRect &sprites_range = m_number_sheet->getSpritesRange();
  const Math::Size &sprites_count = m_number_sheet->getSpritesCount();
  int32 x=0, y=0;
  Math::IRect uv = {0, 0, 0, 0};
  y = n / sprites_count.w;
  x = n % sprites_count.w;
  uv.w = sprites_range.w / sprites_count.w;
  uv.h = sprites_range.h / sprites_count.h;
  uv.x = sprites_range.x + x * uv.w;
  uv.y = sprites_range.y + (sprites_count.h-y-1) * uv.h;
  setUV(uv);
}

void NumberViewer::setNumberSheet(Animation *sheet) {
  m_number_sheet = sheet;
}


// HitGaugeViewer class
HitGaugeViewer::HitGaugeViewer()
:Component(), m_alpaca_index(0), m_alpaca_hit_gauge(nullptr), m_number_viewers()
{
  ;
}

HitGaugeViewer::~HitGaugeViewer() {
  ;
}

void HitGaugeViewer::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("alpacaindex"));
  assert(obj.HasMember("numberviewer"));
  assert(obj["alpacaindex"].IsUint());
  assert(obj["numberviewer"].IsObject());

  m_alpaca_index = obj["alpacaindex"].GetUint();
  auto number_json_object = obj["numberviewer"].GetObject();

  auto *scene = Application::Get().getScene();
  GameObject *new_gameobject = nullptr;
  glm::vec3 position(0.0f, 0.0f, 1.0f);
  auto viewer_count = (int32)m_number_viewers.getMaxSize();
  for(int32 i=0; i<viewer_count; ++i) {
    new_gameobject = scene->createGameObject(number_json_object);
    const glm::vec2 &scale = new_gameobject->getScale();
    position.x = scale.x*(i - viewer_count/2);
    position.x -= viewer_count&1 ? 0.0f : scale.x/2.0f;
    new_gameobject->setParent(getGameObject());
    new_gameobject->setLocalPosition(position);
    m_number_viewers[i] = new_gameobject->getComponent<NumberViewer>();
  }
}

void HitGaugeViewer::start() {
  auto *game_manager = GameManager::GetGlobal();
  auto *alpaca = game_manager->getAlpaca(m_alpaca_index);
  m_alpaca_hit_gauge = (alpaca->getGameObject())->getComponent<HitGauge>();

  for(Uint32 i=0; i<m_number_viewers.getMaxSize(); ++i) {
    m_number_viewers[i]->setNumber(0);
  }

  Component::start();
}

void HitGaugeViewer::update() {
  assert(m_alpaca_hit_gauge);
  auto gauge = (Uint32)m_alpaca_hit_gauge->getGauge();
  size_t viewer_count = m_number_viewers.getMaxSize();
  Uint32 n=0, a=1;
  for(Uint32 i=0; i<viewer_count; ++i) {
    n = (gauge/a)%10;
    m_number_viewers[viewer_count-i-1]->setNumber(n);
    a *= 10;
  }
}

void HitGaugeViewer::release() {
  ;
}

}