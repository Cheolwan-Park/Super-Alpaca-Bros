#include "TitleBanner.h"

namespace Title {
// TitleBanner class
TitleBanner::TitleBanner()
  :Component(), m_elapsed_time(0.0f), m_wait_time(0.0f),
  m_duration(0.0f), m_rotate(0.0f), m_scale(0.0f),
  m_default_rotation(0.0f), m_default_scale(0.0f) {
  ;
}

TitleBanner::~TitleBanner() {
  ;
}

void TitleBanner::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("waittime"));
  assert(obj.HasMember("duration"));
  assert(obj.HasMember("rotate"));
  assert(obj.HasMember("scale"));
  assert(obj["waittime"].IsFloat());
  assert(obj["duration"].IsFloat());
  assert(obj["rotate"].IsFloat());
  assert(obj["scale"].IsFloat());

  m_wait_time = obj["waittime"].GetFloat();
  m_duration = obj["duration"].GetFloat();
  m_rotate = obj["rotate"].GetFloat();
  m_scale = obj["scale"].GetFloat();
}

void TitleBanner::start() {
  m_default_rotation = getRotation();
  m_default_scale = getScale();

  Component::start();
}

void TitleBanner::update() {
  Component::update();

  // banner effect
  Time &t = Time::Get();
  m_elapsed_time += t.getDeltatime();
  float32 theta = 2.0f * Math::Constant::PIf * m_elapsed_time/m_duration;
  float32 sin_x = sinf(theta);
  float32 sin_2x = sinf(2*theta);

  glm::vec2 scale = m_default_scale;
  scale *= (1.0f + sin_2x * m_scale);

  float32 rotation = m_default_rotation;
  rotation += sin_x*m_rotate;

  setScale(scale);
  setRotation(rotation);

  // check input
  if(m_elapsed_time > m_wait_time) {
    SDL::Input &input = SDL::Input::Get();
    if (input.isAnyKeyPressed()) {
      auto *zoom_transition = getGameObject()->getComponent<ZoomTransition>();
      if(!zoom_transition->isTransitioning())
        zoom_transition->transition();
    }
  }
}

void TitleBanner::release() {
  ;
}


// TutorialBanner
TutorialBanner::TutorialBanner()
  :Component(), m_elapsed_time(0.0f), m_duration(0.0f),
   m_each_width(0.0f), m_count(0), m_now(0), m_moving_start_position() {
  ;
}

TutorialBanner::~TutorialBanner() {
  ;
}

void TutorialBanner::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("duration"));
  assert(obj.HasMember("count"));
  assert(obj.HasMember("eachwidth"));
  assert(obj["duration"].IsFloat());
  assert(obj["count"].IsUint());
  assert(obj["eachwidth"].IsFloat());

  m_duration = obj["duration"].GetFloat();
  m_count = obj["count"].GetUint();
  m_each_width = obj["eachwidth"].GetFloat();
}

void TutorialBanner::start() {
  Component::start();
}

void TutorialBanner::update() {
  if(isMoving()) {
    Time &t = Time::Get();
    m_elapsed_time += t.getDeltatime();
    glm::vec3 target_position = m_moving_start_position;
    target_position.x += m_each_width;
    if(m_elapsed_time > m_duration) {
      setLocalPosition(target_position);
      m_elapsed_time = 0.0f;
      setMoving(false);
    } else {
      glm::vec3 position = glm::mix(m_moving_start_position, target_position, m_elapsed_time/m_duration);
      setLocalPosition(position);
    }
  } else {
    SDL::Input &input = SDL::Input::Get();
    if(input.isAnyKeyPressed()) {
      if(m_now < m_count) {
        ++m_now;
        m_moving_start_position = getLocalPosition();
        m_elapsed_time = 0.0f;
        setMoving(true);
      } else {
        auto *zoom_transition = getGameObject()->getComponent<ZoomTransition>();
        if (!zoom_transition->isTransitioning())
          zoom_transition->transition();
      }
    }
  }
}

void TutorialBanner::release() {
  ;
}

int32_t TutorialBanner::isMoving() {
  return m_flags.getFlag(2);
}

void TutorialBanner::setMoving(int32_t val) {
  m_flags.setFlag(2, val);
}


// Rotator class
Rotator::Rotator()
  :Component(), m_speed(0.0f) {

}

Rotator::~Rotator() {
  ;
}

void Rotator::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("speed"));
  assert(obj["speed"].IsFloat());

  m_speed = obj["speed"].GetFloat();
}

void Rotator::start() {
  Component::start();
}

void Rotator::update() {
  Component::update();

  Time &t = Time::Get();
  rotate(m_speed * t.getDeltatime());
}

void Rotator::release() {
  ;
}

float32 Rotator::getSpeed() const {
  return m_speed;
}

void Rotator::setSpeed(float32 v) {
  m_speed = v;
}


// ScaleRepeater class
ScaleRepeater::ScaleRepeater()
  :Component(), m_elapsed_time(0.0f), m_duration(0.0f),
  m_scale(0.0f), m_default_scale(0.0f ) {
  ;
}

ScaleRepeater::~ScaleRepeater() {
  ;
}

void ScaleRepeater::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("duration"));
  assert(obj.HasMember("scale"));
  assert(obj["duration"].IsFloat());
  assert(obj["scale"].IsFloat());

  m_duration = obj["duration"].GetFloat();
  m_scale = obj["scale"].GetFloat();
}

void ScaleRepeater::start() {
  m_default_scale = getScale();
  Component::start();
}

void ScaleRepeater::update() {
  Component::update();

  Time &t = Time::Get();
  m_elapsed_time += t.getDeltatime();
  float32 theta = 2.0f * Math::Constant::PIf * m_elapsed_time/m_duration;

  glm::vec2 scale = m_default_scale;
  scale *= (1.0f + sin(theta) * m_scale);

  setScale(scale);
}

void ScaleRepeater::release() {
  ;
}

float32 ScaleRepeater::getDuration() const {
  return m_duration;
}

float32 ScaleRepeater::getMaxScale() const {
  return m_scale;
}

void ScaleRepeater::setDuration(float32 duration) {
  m_duration = duration;
}

void ScaleRepeater::setMaxScale(float32 scale) {
  m_scale = scale;
}


// ZoomTransition class
ZoomTransition::ZoomTransition()
  :Component(), m_load_scene(""),
  m_duration(0.0f), m_elapsed_time(0.0f),
  m_target_ltn(0.0f), m_target_rbf(0.0f),
  m_default_ltn(0.0f), m_default_rbf(0.0f),
  m_camera(nullptr) {
  ;
}

ZoomTransition::~ZoomTransition() {
  ;
}

void ZoomTransition::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("loadscene"));
  assert(obj.HasMember("duration"));
  assert(obj.HasMember("target_ltn"));
  assert(obj.HasMember("target_rbf"));
  assert(obj["loadscene"].IsString());
  assert(obj["duration"].IsFloat());
  assert(obj["target_ltn"].IsObject());
  assert(obj["target_rbf"].IsObject());

  m_load_scene = obj["loadscene"].GetString();
  m_duration = obj["duration"].GetFloat();
  JsonParseMethods::ReadVector(obj["target_ltn"].GetObject(), &m_target_ltn);
  JsonParseMethods::ReadVector(obj["target_rbf"].GetObject(), &m_target_rbf);
}

void ZoomTransition::start() {
  auto *scene = Application::Get().getScene();
  m_camera = scene->getCamera("main"_hash);
  assert(m_camera);
  m_default_ltn = m_camera->getLeftTopNear();
  m_default_rbf = m_camera->getRightBottomFar();

  Component::start();
}

void ZoomTransition::update() {
  Component::update();
  if(isTransitioning()) {
    Time &t = Time::Get();
    m_elapsed_time += t.getDeltatime();
    float32 percentage = m_elapsed_time/m_duration;

    glm::vec3 ltn = glm::mix(m_default_ltn, m_target_ltn, percentage);
    glm::vec3 rbf = glm::mix(m_default_rbf, m_target_rbf, percentage);

    m_camera->setProjection(ltn.x, rbf.x, rbf.y, ltn.y, ltn.z, rbf.z);

    if(percentage > 1.0f) {
      auto &app = Application::Get();
      app.setScene(m_load_scene.c_str());
    }
  }
}

void ZoomTransition::release() {
  ;
}

void ZoomTransition::transition() {
  m_elapsed_time = 0.0f;
  m_flags.setFlag(2, true);
}

int32_t ZoomTransition::isTransitioning() const {
  return m_flags.getFlag(2);
}

}