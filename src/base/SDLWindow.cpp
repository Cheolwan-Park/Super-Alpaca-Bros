#include "SDLWindow.hpp"

namespace SDL
{
    // Window class
    Window::Window()
    :m_window(nullptr)
    {
        ;
    }
    
    Window::Window(const Window &other)
    :m_window(other.m_window)
    {
        ;
    }
    
    Window::~Window()
    {
        ;
    }
    
    Window &Window::operator=(const Window &other)
    {
        assert(this != &other);
        this->m_window = other.m_window;
        return (*this);
    }
    
    GLint Window::Create(GLuint w, GLuint h,
                         const GLchar *title)
    {
        m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 
                                    SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
        assert(m_window);

        return true;
    }
    
    void Window::Destroy()
    {
        if(m_window)
        {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
    }
    
    void Window::Swap()
    {
        if(isAvailable())
        {
            SDL_GL_SwapWindow(m_window);
        }
    }
    
    GLint Window::SetCurrentContext()
    {
        if(isAvailable())
        {
            SDL::glcontext = SDL_GL_CreateContext(m_window);
            assert(SDL::glcontext);
            return SDL_TRUE;
        }
        else 
            return SDL_FALSE;
    }
    
    void Window::SetSize(GLuint width, GLuint height)
    {
        if(isAvailable())
        {
            SDL_SetWindowSize(m_window, width, height);
        }
    }

    void Window::SetFullscreen(Uint32 flag)
    {
        if(isAvailable())
        {
            SDL_SetWindowFullscreen(m_window, flag);
        }
    }

    void Window::SetWindowIcon(SDL_Surface *img)
    {
        if(isAvailable())
        {
            SDL_SetWindowIcon(m_window, img);
        }
    }

    void Window::SetWindowTitle(const char *title)
    {
        if(isAvailable())
        {
            SDL_SetWindowTitle(m_window, title);
        }
    }
    
    GLint Window::isAvailable()const
    {
        return (m_window != nullptr);
    }
    
    void Window::GetSize(GLint *width, GLint *height)const
    {
        if(isAvailable())
        {
            SDL_GetWindowSize(m_window, width, height);
        }
    }
}





