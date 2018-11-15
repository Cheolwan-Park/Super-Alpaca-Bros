#include "SDLWindow.hpp"

namespace SDL {
// Window class
Window::Window()
    : m_window(nullptr) {
  ;
}

Window::Window(const Window &other)
    : m_window(other.m_window) {
  ;
}

Window::~Window() {
  ;
}

Window &Window::operator=(const Window &other) {
  assert(this != &other);
  this->m_window = other.m_window;
  return (*this);
}

GLint Window::create(GLuint w, GLuint h,
                     const GLchar *title) {
  m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
  assert(m_window);

  return true;
}

void Window::destroy() {
  if (m_window) {
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
  }
}

void Window::swap() {
  if (isAvailable()) {
    SDL_GL_SwapWindow(m_window);
  }
}

GLint Window::setCurrentContext() {
  if (isAvailable()) {
    SDL::gl_context = SDL_GL_CreateContext(m_window);
    assert(SDL::gl_context);

    // to fix temporaily https://bugzilla.libsdl.org/show_bug.cgi?id=4272
    SDL_PumpEvents();
    GLint w=0, h=0;
    getSize(&w, &h);
    setSize(w, h);

    return SDL_TRUE;
  } else
    return SDL_FALSE;
}

void Window::setSize(GLint width, GLint height) {
  if (isAvailable()) {
    SDL_SetWindowSize(m_window, width, height);
  }
}

void Window::setFullscreen(Uint32 flag) {
  if (isAvailable()) {
    SDL_SetWindowFullscreen(m_window, flag);
  }
}

void Window::setWindowIcon(SDL_Surface *img) {
  if (isAvailable()) {
    SDL_SetWindowIcon(m_window, img);
  }
}

void Window::setWindowTitle(const char *title) {
  if (isAvailable()) {
    SDL_SetWindowTitle(m_window, title);
  }
}

GLint Window::isAvailable() const {
  return (m_window != nullptr);
}

void Window::getSize(GLint *width, GLint *height) const {
  if (isAvailable()) {
    SDL_GetWindowSize(m_window, width, height);
  }
}
}





