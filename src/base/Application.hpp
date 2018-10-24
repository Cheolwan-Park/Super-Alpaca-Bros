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
#include "Animation.hpp"
#include <thread>

namespace Base
{
    class Application;
    
    typedef GLint(*GLSettingFun)(void);
    typedef int32(*AppSettingFun)(void);
    typedef void(*AppReleaseFun)(void);
    
    class ObjectScene;
    
    class Application
    {
    private:
        Application(const Application &other) = delete;
        Application &operator=(const Application &other) = delete;
        
        Application();
        
    public:
        ~Application();
        
        int32 Create(SDL::Window window, GLSettingFun gl_setting);
        
        void Run();
        
        void Quit();
        
        void Release(AppReleaseFun fun);
        
        // allocator is pointer of that used by scene
        // mark is pointer to release scene
        int32 SetScene(ObjectScene *scene);
        int32 SetScene(ObjectScene *scene, StackAllocator::Marker mark);

        // set scene with scene file name
        // create JsonScene
        int32 SetScene(const char *filename);
        
        SDL::Window GetWindow();
        
        ObjectScene *GetScene();

        StackAllocator &GetAllocator();

        Storage<Texture> &GetTextureStorage();

        Storage<ShaderProgram> &GetShaderStorage();

        Storage<Animation> &GetAnimationStorage();
        
        int32 isQuit();
        
        static Application &Get();
        
    private:
        SDL::Window m_window;
        ObjectScene *m_scene;
        StackAllocator::Marker m_release_marker;
        int32 m_quit;
        StackAllocator m_allocator;
        Storage<Texture> m_texturestorage;
        Storage<ShaderProgram> m_shaderstorage;
        Storage<Animation> m_animationstorage;
    };
}


#endif
