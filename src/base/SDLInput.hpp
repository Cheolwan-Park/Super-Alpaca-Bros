#ifndef BaseSDLInput_hpp
#define BaseSDLInput_hpp

#include "SDL.hpp"

namespace SDL {
class Input {
 public:
  static constexpr GLuint NUM_MOUSE_BUTTONS = 8;
  struct Mouse {
    GLint x;
    GLint y;
    GLchar button_down_buf[NUM_MOUSE_BUTTONS];
    GLchar button_pressed_buf[NUM_MOUSE_BUTTONS];
  };

 private:
  static constexpr GLuint KEYBOARD_BUFFER_SIZE = SDL_NUM_SCANCODES;
  struct Data {
    GLchar key_down_buf[KEYBOARD_BUFFER_SIZE];
    GLchar key_pressed_buf[KEYBOARD_BUFFER_SIZE];
    Mouse mouse_info;
    SDL::Window *window;
  };

  Input();

 public:
  Input(const Input &other) = delete;

  Input &operator=(const Input &other) = delete;

  ~Input();

  void clear();

  void pollEvents();

  GLint isAnyKeyPressed() const;

  GLint isKeyDown(GLint key) const;

  GLint isKeyPressed(GLint key) const;

  GLint isButtonDown(GLuint button) const;

  GLint isButtonPressed(GLint button) const;

  void getMousePos(GLdouble *x, GLdouble *y) const;

  const Mouse &getMouseInfo() const;

  // callbacks
 private:
  void handleEvent(const SDL_Event &event);

 public:
  static Input &Get();

 private:
  Data m_data;
};
}

#endif
