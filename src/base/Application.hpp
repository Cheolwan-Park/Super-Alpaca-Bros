#ifndef BaseApplication_hpp
#define BaseApplication_hpp

#include "types.hpp"
#include "SDL.hpp"
#include "SDLWindow.hpp"
#include "Allocator.hpp"
#include "TextureStorage.hpp"
#include "ShaderProgram.hpp"
#include "BitFlag.hpp"
#include "Storage.hpp"
#include <thread>

namespace Base
{
    class Application;
    
    typedef GLint(*GLSettingFun)(void);
    typedef int32(*AppSettingFun)(void);
    typedef void(*AppReleaseFun)(void);
    
    class Scene;
    
    class Application
    {
    private:
        Application(const Application &other) = delete;
        Application &operator=(const Application &other) = delete;
        
        Application();
        
    public:
        ~Application();
        
        int32 Create(SDL::Window window,
                     GLSettingFun gl_setting,
                     AppSettingFun app_setting);
        
        void Run();
        
        void Quit();
        
        void Release(AppReleaseFun fun);
        
        // allocator is pointer of that used by scene
        // mark is pointer to release scene
        int32 SetScene(Scene *scene, StackAllocator::Marker mark = 0);
        
        SDL::Window GetWindow();
        
        Scene *GetScene();

        StackAllocator &GetAllocator();

        Storage<Texture> &GetTextureStorage();

        Storage<ShaderProgram> &GetShaderStorage();
        
        int32 isQuit();
        
        static Application &Get();
        
    private:
        SDL::Window m_window;
        Scene *m_scene;
        StackAllocator::Marker m_release_marker;
        int32 m_quit;
        StackAllocator m_allocator;
        Storage<Texture> m_texturestorage;
        Storage<ShaderProgram> m_shaderstorage;
    };
}


#endif
