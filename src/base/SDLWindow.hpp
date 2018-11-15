#ifndef BaseSDLWindow_hpp
#define BaseSDLWindow_hpp

#include "SDL.hpp"
#include "types.hpp"

namespace SDL {
class Window {
 public:
  Window();

  Window(const Window &other);

  ~Window();

  Window &operator=(const Window &other);

  GLint create(GLuint width, GLuint height,
               const GLchar *title);

  void destroy();

  void swap();

  GLint setCurrentContext();

  void setSize(GLint width, GLint height);

  void setFullscreen(Uint32 flag);

  void setWindowIcon(SDL_Surface *img);

  void setWindowTitle(const char *title);

  GLint isAvailable() const;

  void getSize(GLint *width, GLint *height) const;

 private:
  SDL_Window *m_window;
};
}

#endif
