#include "HitGauge.hpp"

namespace Game {
// HitGauge class
HitGauge::HitGauge()
    : Component(), m_gauge(0.0f), m_max_gauge(0.0f),
      m_min_factor(0.0f), m_max_factor(0.0f) {
  ;
}

HitGauge::HitGauge(const HitGauge &other)
    : Component(other), m_gauge(other.m_gauge), m_max_gauge(other.m_max_gauge),
      m_min_factor(other.m_min_factor), m_max_factor(other.m_max_factor) {
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
  return (*this);
}

void HitGauge::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("maxgauge"));
  assert(obj.HasMember("minfactor"));
  assert(obj.HasMember("maxfactor"));
  assert(obj["maxgauge"].IsFloat());
  assert(obj["minfactor"].IsFloat());
  assert(obj["maxfactor"].IsFloat());

  m_max_gauge = obj["maxgauge"].GetFloat();
  m_min_factor = obj["minfactor"].GetFloat();
  m_max_factor = obj["maxfactor"].GetFloat();
}

void HitGauge::start() {
  Component::start();
}

void HitGauge::update() {
  ;
}

void HitGauge::release() {
  ;
}

void HitGauge::resetGauge() {
  m_gauge = 0.0f;
}

void HitGauge::gaugeUp(float32 val) {
  m_gauge += val;
  m_gauge = m_gauge > m_max_gauge ? m_max_gauge : m_gauge;
}

float32 HitGauge::getGauge() const {
  return m_gauge;
}

float32 HitGauge::GetFactor() const {
  float32 percentage = m_gauge / m_max_gauge;
  return (m_min_factor + (m_max_factor - m_min_factor) * percentage * percentage);
}
//
//// NumberViewer class
//NumberViewer::NumberViewer()
//    : Sprite(), m_animation(nullptr) {
//  ;
//}
//
//NumberViewer::NumberViewer(const NumberViewer &other)
//    : Sprite(other), m_animation(other.m_animation) {
//  ;
//}
//
//NumberViewer::~NumberViewer() {
//  ;
//}
//
//NumberViewer &NumberViewer::operator=(const NumberViewer &other) {
//  Sprite::operator=(other);
//  this->m_animation = other.m_animation;
//  return (*this);
//}
//
//void NumberViewer::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
//
//}
}