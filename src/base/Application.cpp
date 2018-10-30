#include "Application.hpp"
#include "Time.hpp"
#include "types.hpp"
#include "SDLInput.hpp"
#include "Sprite.hpp"
#include "Scene.hpp"


namespace Base
{
    
    // Application class
    
    Application::Application()
    :m_window(), m_scene(nullptr), m_release_marker(0), m_quit(false), 
    m_allocator(), m_texturestorage(), m_shaderstorage(), m_animationstorage()
    {
        m_texturestorage.SetFreeFunc(FreeTexture);
        m_shaderstorage.SetFreeFunc(FreeShader);
        m_animationstorage.SetFreeFunc(FreeAnimation);
    }
    
    Application::~Application()
    {
        ;
    }
    
    int32 Application::Create(SDL::Window window, GLSettingFun gl_setting)
    {
        if(false == window.isAvailable())
            return RET_FAILED;
        m_window = window;
        m_window.SetCurrentContext();
        
        if(SDL_FALSE == SDL::InitGL(gl_setting))
            return RET_FAILED;
        
        printf("==== OpenGL Info ====\n");
        printf("OpenGL version : %s\n",   glGetString(GL_VERSION));
        printf("GLSL version   : %s\n",   glGetString(GL_SHADING_LANGUAGE_VERSION));
        printf("Vendor         : %s\n",   glGetString(GL_VENDOR));
        printf("Renderer       : %s\n", glGetString(GL_RENDERER));
        
        return RET_SUCC;
    }
    
    void Application::Run()
    {
        Time &time = Time::Get();
        time.SetIdealDeltatime(Time::milliseconds(1000/60));
        
        SDL::Input &input = SDL::Input::Get();

        while (!m_quit)
        {
            input.PollEvents();
            glClear(GL_COLOR_BUFFER_BIT);
            if(m_scene)
            {
                m_scene->CheckDeletedObjects();
                m_scene->Update();
                m_scene->Render();
            }
            m_window.Swap();
            time.Update();
            input.Clear();
        }
    }
    
    void Application::Quit()
    {
        m_quit = true;
    }
    
    void Application::Release(AppReleaseFun fun)
    {
        m_scene->~Scene();
        m_release_marker = m_allocator.GetTopMarker();
        m_texturestorage.Clear();
        m_shaderstorage.Clear();
        if(fun)
            fun();
        SDL::Quit();
    }

    int32 Application::SetScene(const FileIO &f)
    {
        if(m_scene)
        {
            m_scene->~Scene();
            m_allocator.FreeWithMarker(m_release_marker);
        }

        m_release_marker = m_allocator.GetTopMarker();
        m_texturestorage.Clear();
        m_shaderstorage.Clear();
        
        m_scene = new (m_allocator.Alloc<Scene>()) Scene();
        assert(m_scene);
        m_scene->SetJsonFile(f);
        return m_scene->Init();
    }

    int32 Application::SetScene(const char *filename)
    {
        if(m_scene)
        {
            m_scene->~Scene();
            m_allocator.FreeWithMarker(m_release_marker);
        }

        m_release_marker = m_allocator.GetTopMarker();
        m_texturestorage.Clear();
        m_shaderstorage.Clear();
        
        m_scene = new (m_allocator.Alloc<Scene>()) Scene();
        assert(m_scene);
        m_scene->SetJsonFile(filename);
        return m_scene->Init();
    }
    
    SDL::Window Application::GetWindow()
    {
        return m_window;
    }
    
    Scene *Application::GetScene()
    {
        return m_scene;
    }

    StackAllocator &Application::GetAllocator()
    {
        return m_allocator;
    }

    Storage<Texture> &Application::GetTextureStorage()
    {
        return m_texturestorage;
    }

    Storage<ShaderProgram> &Application::GetShaderStorage()
    {
        return m_shaderstorage;
    }

    Storage<Animation> &Application::GetAnimationStorage()
    {
        return m_animationstorage;
    }
    
    int32 Application::isQuit()
    {
        return m_quit;
    }
    
    Application &Application::Get()
    {
        static Application instance;
        return instance;
    }
}
