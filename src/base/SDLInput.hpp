#ifndef BaseSDLInput_hpp
#define BaseSDLInput_hpp

#include "SDL.hpp"

namespace SDL
{
    class Input
    {
    public:
        static constexpr GLuint NUM_MOUSE_BUTTONS = 8;
        struct Mouse
        {
            GLint x;
            GLint y;
            GLchar buttondown_buf[NUM_MOUSE_BUTTONS];
            GLchar buttonpressed_buf[NUM_MOUSE_BUTTONS];
        };
        
    private:
        static constexpr GLuint KEYBOARD_BUFFER_SIZE = SDL_NUM_SCANCODES;
        struct Data
        {
            GLchar keydown_buf[KEYBOARD_BUFFER_SIZE];
            GLchar keypressed_buf[KEYBOARD_BUFFER_SIZE];
            Mouse mouseinfo;
        };
        
        Input();
        
        Input(const Input &other) = delete;
        
        Input &operator=(const Input &other) = delete;
        
    public:
        ~Input();
        
        void Clear();
        
        void PollEvents();
        
        GLint isKeyDown(GLint key)const;
        
        GLint isKeyPressed(GLint key)const;
        
        GLint isButtonDown(GLuint button)const;
        
        GLint isButtonPressed(GLint button)const;
        
        void GetMousePos(GLdouble *x, GLdouble *y)const;
        
        const Mouse &GetMouseInfo()const;
        
        // callbacks
    private:
        void HandleEvent(const SDL_Event &event);
        
    public:
        static Input &Get();
        
    private:
        Data m_data;
    };
}

#endif
