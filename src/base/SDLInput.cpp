#include "SDLInput.hpp"
#include "SDLWindow.hpp"
#include "Application.hpp"
#include <string.h>

namespace SDL
{
    // Input class
    
    Input::Input()
    :m_data()
    {
        memset(&m_data, 0, sizeof(Data));
    }
    
    Input::~Input()
    {
        ;
    }
    
    void Input::Clear()
    {
        memset(m_data.keypressed_buf, 0, sizeof(m_data.keypressed_buf));
        memset(m_data.mouseinfo.buttonpressed_buf, 0,
               sizeof(m_data.mouseinfo.buttonpressed_buf));
    }
    
    void Input::PollEvents()
    {
        static SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            HandleEvent(event);
        }
    }
    
    GLint Input::isKeyDown(GLint key)const
    {
        return m_data.keydown_buf[key];
    }
    
    GLint Input::isKeyPressed(GLint key)const
    {
        return m_data.keypressed_buf[key];
    }
    
    GLint Input::isButtonDown(GLuint button)const
    {
        return m_data.mouseinfo.buttondown_buf[button];
    }
    
    GLint Input::isButtonPressed(GLint button)const
    {
        return m_data.mouseinfo.buttonpressed_buf[button];
    }
    
    void Input::GetMousePos(GLdouble *x, GLdouble *y)const
    {
        *x = m_data.mouseinfo.x;
        *y = m_data.mouseinfo.y;
    }
    
    const Input::Mouse &Input::GetMouseInfo()const
    {
        return m_data.mouseinfo;
    }

    void Input::HandleEvent(const SDL_Event &event)
    {
        switch(event.type)
        {
            case SDL_QUIT:
            {
                Base::Application::Get().Quit();
                break;
            }
            case SDL_KEYDOWN:
            {
                m_data.keydown_buf[event.key.keysym.scancode] = SDL_TRUE;
                m_data.keypressed_buf[event.key.keysym.scancode] = SDL_TRUE;
                break;
            }

            case SDL_KEYUP:
            {
                m_data.keydown_buf[event.key.keysym.scancode] = SDL_FALSE;
                break;
            }

            case SDL_MOUSEMOTION:
            {
                SDL_GetMouseState(&m_data.mouseinfo.x, &m_data.mouseinfo.y);
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
                SDL_GetMouseState(&m_data.mouseinfo.x, &m_data.mouseinfo.y);
                m_data.mouseinfo.buttondown_buf[event.button.button] = SDL_TRUE;
                m_data.mouseinfo.buttonpressed_buf[event.button.button] = SDL_TRUE;
                break;
            }
            
            case SDL_MOUSEBUTTONUP:
            {
                SDL_GetMouseState(&m_data.mouseinfo.x, &m_data.mouseinfo.y);
                m_data.mouseinfo.buttondown_buf[event.button.button] = SDL_FALSE;
                break;
            }

            default: break;
        }
    }
    
    Input &Input::Get()
    {
        static Input instance;
        return instance;
    }
}






