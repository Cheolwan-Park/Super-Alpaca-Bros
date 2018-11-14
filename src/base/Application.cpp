#include "Application.hpp"
#include "Time.hpp"
#include "types.hpp"
#include "SDLInput.hpp"
#include "Sprite.hpp"
#include "Scene.hpp"

namespace Base {

// Application class

Application::Application()
    : m_window(), m_scene(nullptr), m_release_marker(0), m_quit(false), m_scene_changed(false),
      m_allocator(), m_texture_storage(), m_shader_storage(), m_animation_storage() {
  m_texture_storage.setFreeFunc(FreeTexture);
  m_shader_storage.setFreeFunc(FreeShader);
  m_animation_storage.setFreeFunc(FreeAnimation);
}

Application::~Application() {
  ;
}

int32 Application::create(SDL::Window window, GLSettingFun gl_setting) {
  if (!window.isAvailable())
    return RET_FAILED;
  m_window = window;
  m_window.setCurrentContext();

  if (SDL_FALSE == SDL::InitGL(gl_setting))
    return RET_FAILED;

  printf("==== OpenGL Info ====\n");
  printf("OpenGL version : %s\n", glGetString(GL_VERSION));
  printf("GLSL version   : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("Vendor         : %s\n", glGetString(GL_VENDOR));
  printf("Renderer       : %s\n", glGetString(GL_RENDERER));

  return RET_SUCC;
}

void Application::run() {
  Time &time = Time::Get();
  time.setIdealDeltatime(Time::milliseconds(1000 / 60));

  SDL::Input &input = SDL::Input::Get();

  while (!isQuit()) {
    input.pollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    if (m_scene) {
      m_scene->checkDeletedObjects();
      m_scene_changed = false;
      m_scene->update();
      if(!m_scene_changed)
        m_scene->render();
    }
    m_window.swap();
    time.update();
    input.clear();
  }
}

void Application::quit() {
  m_quit = true;
}

void Application::release(AppReleaseFun fun) {
  m_scene->~Scene();
  m_texture_storage.clear();
  m_shader_storage.clear();
  if (fun)
    fun();
  SDL::Quit();
}

int32 Application::setScene(const FileIO &f) {
  if (m_scene) {
    m_scene->~Scene();
    m_allocator.freeWithMarker(m_release_marker);
  }

  m_release_marker = m_allocator.getTopMarker();
  m_texture_storage.clear();
  m_shader_storage.clear();

  m_scene = new(m_allocator.alloc<Scene>()) Scene();
  assert(m_scene);
  m_scene->setJsonFile(f);
  m_scene_changed = true;
  return m_scene->init();
}

int32 Application::setScene(const char *filename) {
  if (m_scene) {
    m_scene->~Scene();
    m_allocator.freeWithMarker(m_release_marker);
  }

  m_release_marker = m_allocator.getTopMarker();
  m_texture_storage.clear();
  m_shader_storage.clear();

  m_scene = new(m_allocator.alloc<Scene>()) Scene();
  assert(m_scene);
  m_scene->setJsonFile(filename);
  m_scene_changed = true;
  return m_scene->init();
}

SDL::Window Application::getWindow() {
  return m_window;
}

Scene *Application::getScene() {
  return m_scene;
}

StackAllocator &Application::getAllocator() {
  return m_allocator;
}

Storage<Texture> &Application::getTextureStorage() {
  return m_texture_storage;
}

Storage<ShaderProgram> &Application::getShaderStorage() {
  return m_shader_storage;
}

Storage<Animation> &Application::getAnimationStorage() {
  return m_animation_storage;
}

int32 Application::isQuit() {
  return m_quit;
}

Application &Application::Get() {
  static Application instance;
  return instance;
}
}
