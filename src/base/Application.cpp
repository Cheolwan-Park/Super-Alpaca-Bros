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
    :m_window(), m_scene(nullptr), 
    m_release_marker(0), m_quit(false),
    m_allocator(), m_texturestorage(), m_shaderstorage()
    {
        m_texturestorage.SetFreeFunc(FreeTexture);
        m_shaderstorage.SetFreeFunc(FreeShader);
    }
    
    Application::~Application()
    {
        ;
    }
    
    int32 Application::Create(SDL::Window window,
                              GLSettingFun gl_setting,
                              AppSettingFun app_setting)
    {
        if(false == window.isAvailable())
            return RET_FAILED;
        
        m_window = window;
        m_window.SetCurrentContext();
        
        SDL::InitGL(gl_setting);
        
        printf("==== OpenGL Info ====\n");
        printf("OpenGL version : %s\n",   glGetString(GL_VERSION));
        printf("GLSL version   : %s\n",   glGetString(GL_SHADING_LANGUAGE_VERSION));
        printf("Vendor         : %s\n",   glGetString(GL_VENDOR));
        printf("Renderer       : %s\n", glGetString(GL_RENDERER));
        
        if(app_setting)
            return app_setting();
        else
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
        if(fun)
            fun();
        SDL::Quit();
    }
    
    int32 Application::SetScene(Scene *scene, StackAllocator::Marker mark)
    {
        assert(scene);
        
        if(nullptr != m_scene)
        {
            delete m_scene;
            m_allocator.FreeWithMarker(m_release_marker);
        }
        m_texturestorage.Clear();
        m_shaderstorage.Clear();
        m_scene = scene;
        
        if(0 == mark)
            m_release_marker = m_allocator.GetTopMarker();
        else
            m_release_marker = mark;
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
