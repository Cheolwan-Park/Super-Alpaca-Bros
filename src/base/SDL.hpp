#ifndef BaseSDL_hpp
#define BaseSDL_hpp

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif
#include <GL/glew.h>

namespace SDL {
// class definition
class Window;
class Input;

extern SDL_GLContext gl_context;

// general functions
typedef GLint(*SDL_setting_func)(void);
GLint InitSDL(SDL_setting_func setting_fun);

typedef GLint(*GL_setting_func)(void);
GLint InitGL(GL_setting_func setting_fun);

void Quit();
}

#endif
