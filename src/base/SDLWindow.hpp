#ifndef BaseSDLWindow_hpp
#define BaseSDLWindow_hpp

#include "SDL.hpp"
#include "types.hpp"

namespace SDL
{
    class Window
    {
    public:
        Window();
        
        Window(const Window &other);
        
        ~Window();
        
        Window &operator=(const Window &other);
        
        GLint Create(GLuint width, GLuint height,
                     const GLchar *title);
        
        void Destroy();
        
        void Swap();
        
        GLint SetCurrentContext();
        
        void SetSize(GLuint width, GLuint height);

        void SetFullscreen(Uint32 flag);

        void SetWindowIcon(SDL_Surface *img);

        void SetWindowTitle(const char *title);
        
        GLint isAvailable()const;
        
        void GetSize(GLint *width, GLint *height)const;
        
    private:
        SDL_Window *m_window;
    };
}

#endif
