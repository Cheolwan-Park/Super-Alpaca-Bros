#include "SDL.hpp"
#include <stdio.h>

namespace SDL {
SDL_GLContext gl_context = nullptr;

GLint InitSDL(SDL_setting_func setting_fun) {
  Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS;
  if (SDL_Init(flags) < 0)
    return SDL_FALSE;
  if (IMG_Init(IMG_INIT_PNG) < 0)
    return SDL_FALSE;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  if (setting_fun)
    return setting_fun();
  else
    return SDL_TRUE;
}

GLint InitGL(GL_setting_func setting_fun) {
  if (!gl_context) {
    printf("failed InitGL() there is no Opengl Context\n");
    return SDL_FALSE;
  }

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    printf("failed glewInit() : %s\n", glewGetErrorString(err));
    return SDL_FALSE;
  }

  if (!GLEW_VERSION_4_1) {
    printf("OpenGL 4.1 is not available\n");
    return SDL_FALSE;
  }

  // use vsync (if not using vsync, delete under line)
  if (SDL_GL_SetSwapInterval(1) < 0) {
    printf("unable to use vsync : %s\n", SDL_GetError());
    return SDL_FALSE;
  }

  if (setting_fun)
    return setting_fun();
  else
    return SDL_TRUE;
}

void Quit() {
  IMG_Quit();
  SDL_Quit();
}
}
