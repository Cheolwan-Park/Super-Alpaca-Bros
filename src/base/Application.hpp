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

namespace Base {
class Application;

typedef GLint(*GLSettingFun)(void);
typedef int32_t(*AppSettingFun)(void);
typedef void(*AppReleaseFun)(void);

class Scene;

class Application {
 private:
  Application(const Application &other) = delete;
  Application &operator=(const Application &other) = delete;

  Application();

 public:
  ~Application();

  int32_t create(SDL::Window window, GLSettingFun gl_setting);

  void run();

  void quit();

  void release(AppReleaseFun fun);

  int32_t setScene(const FileIO &f);
  int32_t setScene(const char *filename);

  SDL::Window getWindow();

  Scene *getScene();

  StackAllocator &getAllocator();

  Storage<Texture> &getTextureStorage();

  Storage<ShaderProgram> &getShaderStorage();

  Storage<Animation> &getAnimationStorage();

  Storage<Mix_Chunk> &getChunkStorage();

  int32_t isQuit();

  int32_t isSceneChanged();

  static Application &Get();

 private:
  SDL::Window m_window;
  Scene *m_scene;
  StackAllocator::Marker m_release_marker;
  int32_t m_quit;
  int32_t m_scene_changed;
  StackAllocator m_allocator;
  Storage<Texture> m_texture_storage;
  Storage<ShaderProgram> m_shader_storage;
  Storage<Animation> m_animation_storage;
  Storage<Mix_Chunk> m_chunk_storage;
};
}

#endif
