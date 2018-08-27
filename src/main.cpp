#include "Application.hpp"
#include "SDLWindow.hpp"
#include "SDLInput.hpp"
#include "Scene.hpp"
#include "Allocator.hpp"
#include "Sprite.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "JsonScene.hpp"
using namespace Base;

GLint GLSetting();
int32 AppSetting();

int main()
{
    SDL::InitSDL(nullptr);
    Application &app = Application::Get();
    SDL::Window window;

    window.Create(640, 480, "buffalos");
    app.Create(window, GLSetting, AppSetting);

    StackAllocator &allocator = app.GetAllocator();
    FILE *f = OpenFile("../res/scene/test.json", "r");
    JsonScene *scene = new (allocator.Alloc<JsonScene>()) JsonScene();
    scene->SetJsonFile(f);
    app.SetScene(scene);
    app.Run();

    app.Release(nullptr);
    
    return 0;
}

GLint GLSetting()
{
    glEnable(GL_TEXTURE_2D);
    glClearColor(1.0f,1.0f,1.0f,0.0f);

    return RET_SUCC;
}

int32 AppSetting()
{
    Application &app = Application::Get();
    StackAllocator &allocator = app.GetAllocator();
    auto &shaders = app.GetShaderStorage();
    auto &textures = app.GetTextureStorage();
    allocator.ReAllocBuffer(1024*50);
    shaders.AssignMemory(allocator.Alloc<ShaderProgram>(8), 8);
    textures.AssignMemory(allocator.Alloc<Texture>(16), 16);

    // set camera
    GLint w=0, h=0;
    app.GetWindow().GetSize(&w, &h);
    OrthoCamera *maincam = new (allocator.Alloc<OrthoCamera>()) OrthoCamera();
    maincam->SetView(glm::vec3(0.0f, 0.0f, 1.0f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));  
    maincam->SetProjection(-0.5f*w, 0.5f*w,
                           -0.5f*h, 0.5f*h,
                           0.1f, 8.0f);
    Camera::SetMain(maincam);

    return RET_SUCC;
}
