#include "CameraController.hpp"
#include "GameManager.hpp"
#include "Alpaca.hpp"

namespace Game {
CameraController::CameraController()
    : Component(), m_game_manager(nullptr),
      m_margin(0.0f), m_camera_speed(0.0f), m_expand_speed(0.0f), m_expanded(0.0f),
      m_view_ratio(), m_min_camera_size(0.0f), m_max_camera_size(0.0f) {
  ;
}

CameraController::~CameraController() {
  ;
}

void CameraController::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("isSmooth"));
  assert(obj.HasMember("margin"));
  assert(obj.HasMember("cammovespeed"));
  assert(obj.HasMember("expandspeed"));
  assert(obj.HasMember("viewratio"));
  assert(obj.HasMember("min_camerarange"));
  assert(obj.HasMember("max_camerarange"));
  assert(obj["isSmooth"].IsBool());
  assert(obj["margin"].IsFloat());
  assert(obj["expandspeed"].IsFloat());
  assert(obj["cammovespeed"].IsFloat());
  assert(obj["viewratio"].IsObject());
  assert(obj["min_camerarange"].IsFloat());
  assert(obj["max_camerarange"].IsFloat());

  setSmooth(obj["isSmooth"].GetBool());
  m_margin = obj["margin"].GetFloat();
  m_camera_speed = obj["cammovespeed"].GetFloat();
  m_expand_speed = obj["expandspeed"].GetFloat();
  JsonParseMethods::ReadVector2(obj["viewratio"].GetObject(), &m_view_ratio);
  m_min_camera_size = obj["min_camerarange"].GetFloat();
  m_max_camera_size = obj["max_camerarange"].GetFloat();
}

void CameraController::start() {
  m_game_manager = getGameObject()->getComponent<GameManager>();
}

void CameraController::update() {
  assert(m_game_manager);

  // fitting camera
  auto *scene = Application::Get().getScene();
  Camera *camera = scene->getCamera("main"_hash);
  Time &t = Time::Get();
  const glm::vec3 &ltn = camera->getLeftTopNear();
  const glm::vec3 &rbf = camera->getRightBottomFar();

  // move cam
  glm::vec3 pos0(0.0f), pos1(0.0f);
  Alpaca::Alpaca *alpaca0 = m_game_manager->getAlpaca(0);
  Alpaca::Alpaca *alpaca1 = m_game_manager->getAlpaca(1);
  m_game_manager->getAlpaca(0)->getWorldPosition(&pos0);
  m_game_manager->getAlpaca(1)->getWorldPosition(&pos1);
  glm::vec3 new_camera_position(0.0f);
  if (alpaca0->isAvailable() && alpaca1->isAvailable()) { new_camera_position = (pos0 + pos1) / 2.0f; }
  else if (alpaca0->isAvailable()) { new_camera_position = pos0; }
  else if (alpaca1->isAvailable()) { new_camera_position = pos1; }
  else { new_camera_position.x = new_camera_position.y = 0.0f; }
  glm::vec3 campos = (ltn + rbf) / 2.0f;
  new_camera_position.z = campos.z = 1.0f;
  glm::vec3 delta = new_camera_position - campos;
  float32 move_distance = t.getDeltatime() * m_camera_speed;
  if (isSmooth()) {
    move_distance *= (-exp(-(delta.x * delta.x + delta.y * delta.y)) + 1.0f);
  }
  if ((delta.x * delta.x + delta.y * delta.y) > move_distance) {
    campos += glm::normalize(delta) * move_distance;
  }

  // setting camera view size
  glm::vec2 camera_size = {0.0f, 0.0f};
  camera_size.x = (abs(pos0.x - pos1.x) + 2 * m_margin) / m_view_ratio.x;
  camera_size.y = (abs(pos0.y - pos1.y) + 2 * m_margin) / m_view_ratio.y;
  float32 now_range = (m_min_camera_size + (m_max_camera_size - m_min_camera_size) * m_expanded);
  float32 target_range = camera_size.x > camera_size.y ? camera_size.x : camera_size.y;
  float32 expand_percentage = m_expand_speed * t.getDeltatime();
  if (isSmooth()) {
    expand_percentage *= (-exp(-(target_range - now_range) * (target_range - now_range)) + 1.0f);
  }
  float32 expand = (m_max_camera_size - m_min_camera_size) * expand_percentage;
  expand = now_range > target_range ? -expand : expand;
  if (target_range > now_range && target_range > now_range + expand) {
    m_expanded += expand_percentage;
  } else if (target_range < now_range && target_range < now_range - expand) {
    m_expanded -= expand_percentage;
  }
  m_expanded = m_expanded < 0.0f ? 0.0f : m_expanded;
  m_expanded = m_expanded > 1.0f ? 1.0f : m_expanded;
  now_range = (m_min_camera_size + (m_max_camera_size - m_min_camera_size) * m_expanded);
  glm::vec2 camera_view_size = m_view_ratio * now_range;
  camera_view_size *= 0.5f;

  camera->setProjection(campos.x - camera_view_size.x,
                        campos.x + camera_view_size.x,
                        campos.y - camera_view_size.y,
                        campos.y + camera_view_size.y,
                        ltn.z, rbf.z);
}

void CameraController::release() {
  ;
}

int32 CameraController::isSmooth() const {
  return m_flags.getFlag(2);
}

void CameraController::setSmooth(int32 val) {
  m_flags.setFlag(2, val);
}
}