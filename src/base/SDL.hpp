#ifndef BaseSDL_hpp
#define BaseSDL_hpp

#define GLFW_INCLUDE_GLCOREARB
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace SDL
{
    // class definition
    class Window;
    class Input;

    extern SDL_GLContext glcontext;
    
    // general functions
    typedef GLint(*SDLsettingfun)(void);
    GLint InitSDL(SDLsettingfun setting_fun);

    typedef GLint(*GLsettingfun)(void);
    GLint InitGL(GLsettingfun setting_fun);
    
    void Quit();
}


#endif
