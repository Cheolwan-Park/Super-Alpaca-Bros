#include "Button.h"
#include "GameManager.hpp"
#include "Banner.h"
#include "TitleBanner.h"

namespace Game {
// Button class
Button::Button()
  :Component(), m_sprite(nullptr), m_default_uv(), m_pressed_uv() {
  ;
}

Button::~Button() {
  ;
}

void Button::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("presseduv"));
  assert(obj["presseduv"].IsObject());

  JsonParseMethods::ReadIRect(obj["presseduv"].GetObject(), &m_pressed_uv);
}

void Button::start() {
  m_sprite = getGameObject()->getComponent<Sprite>();
  assert(m_sprite);
  m_default_uv = m_sprite->getUV();

  Component::start();
}

void Button::update() {
  Component::update();
  glm::vec3 vertexes[2] = {
      {-0.5f, 0.5f, 1.0f},   // top left
      {0.5f, -0.5f, 1.0f}    // bottom right
  };

  glm::mat3x3 model(1.0f);
  getModel(&model);
  vertexes[0] = model * vertexes[0];
  vertexes[1] = model * vertexes[1];

  SDL::Input &input = SDL::Input::Get();
  GLdouble x=0.0, y=0.0;
  input.getMousePos(&x, &y);

  auto &app = Application::Get();
  auto window = app.getWindow();
  auto *scene = app.getScene();
  auto *ui_camera = scene->getCamera("ui"_hash);
  const glm::vec3 &ltn = ui_camera->getLeftTopNear();
  const glm::vec3 &rbf = ui_camera->getRightBottomFar();

  GLint w=0, h=0;
  window.getSize(&w, &h);
  x = x*abs(ltn.x-rbf.x)/(GLdouble)w;
  y = y*abs(ltn.y-rbf.y)/(GLdouble)h;

  if(vertexes[0].x < x && vertexes[1].x > x
  && vertexes[0].y > y && vertexes[1].y < y) {
    if(input.isButtonDown(SDL_BUTTON_LEFT)) {
      setButtonDown(true);
      m_sprite->setUV(m_pressed_uv);
    } else {
      if(isButtonDowned())
        onClicked();
      setButtonDown(false);
      m_sprite->setUV(m_default_uv);
    }
  }
}

void Button::release() {
  ;
}

void Button::onClicked() {
  ;
}

int32_t Button::isButtonDowned() {
  return m_flags.getFlag(2);
}

void Button::setButtonDown(int32_t val) {
  m_flags.setFlag(2, val);
}


// Restart button
RestartButton::RestartButton()
  :Button() {
  ;
}

RestartButton::~RestartButton() {
  ;
}

void RestartButton::onClicked() {
  auto *scene = Application::Get().getScene();
  auto *pause_ui_object = scene->getObject("ui"_hash, "UI_Pause"_hash);
  auto *pause_ui_mover = pause_ui_object->getComponent<BannerMover>();
  pause_ui_mover->hide();

  auto *game_manager = GameManager::GetGlobal();
  game_manager->restart();
}


// ResumeButton class
ResumeButton::ResumeButton()
  :Button() {
  ;
}

ResumeButton::~ResumeButton() {
  ;
}

void ResumeButton::onClicked() {
  auto *scene = Application::Get().getScene();
  auto *pause_ui_object = scene->getObject("ui"_hash, "UI_Pause"_hash);
  auto *pause_ui_mover = pause_ui_object->getComponent<BannerMover>();
  pause_ui_mover->hide();

  auto *game_manager = GameManager::GetGlobal();
  game_manager->resume();
}


// SceneTransitionButton class
SceneTransitionButton::SceneTransitionButton()
  :Button() {
  ;
}

SceneTransitionButton::~SceneTransitionButton() {
  ;
}

void SceneTransitionButton::onClicked() {
  auto *zoom_transition = getGameObject()->getComponent<Title::ZoomTransition>();
  if (!zoom_transition->isTransitioning())
    zoom_transition->transition();
}


}