#ifndef ALPACAS_BUTTON_H
#define ALPACAS_BUTTON_H

#include "base.hpp"

using namespace Base;

namespace Game {
class Button : public Component {
 public:
  COMPONENT(Button);

  Button();

  Button(const Button &other) = delete;

  ~Button() override;

  Button &operator=(const Button &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  virtual void onClicked();

 private:
  int32_t isButtonDowned();

  void setButtonDown(int32_t val);

 private:
  Sprite *m_sprite;
  Math::Rect m_default_uv;
  Math::IRect m_pressed_uv;

  /*
   * flags
   * Component's flags
   * 2 : isButtonDowned
   */
};

class RestartButton : public Button {
 public:
  COMPONENT(RestartButton);

  RestartButton();

  RestartButton(const RestartButton &other) = delete;

  ~RestartButton() override;

  RestartButton &operator=(const RestartButton &other) = delete;

  void onClicked() override;
};

class ResumeButton : public Button {
 public:
  COMPONENT(ResumeButton);

  ResumeButton();

  ResumeButton(const ResumeButton &other) = delete;

  ~ResumeButton() override;

  ResumeButton &operator=(const ResumeButton &other) = delete;

  void onClicked() override;
};

class SceneTransitionButton : public Button {
 public:
  COMPONENT(SceneTransitionButton);

  SceneTransitionButton();

  SceneTransitionButton(const SceneTransitionButton &other) = delete;

  ~SceneTransitionButton() override;

  SceneTransitionButton &operator=(const SceneTransitionButton &other) = delete;

  void onClicked() override;
};
}

#endif //ALPACAS_BUTTON_H
