#include "base.hpp"
#include "game.hpp"
#include "title.h"

#include <steam/steam_api.h>

using namespace Base;

int32_t JsonAppInit(const char *filename);
void AddFactoryFuncs();

int main(int argc, char* argv[]) {
  SDL::InitSDL(nullptr);

//  if(!SteamAPI_Init()) {
//    fprintf(stderr, "failed to init steam api\n");
//    return -1;
//  }

  String128 path(Directories::res);
  path += "init.json";
  if (RET_FAILED == JsonAppInit(path.c_str()))
    return -1;

  Application &app = Application::Get();
  app.run();

#ifndef NDEBUG
  printf("%lu byte used\n", app.getAllocator().getTopMarker());
#endif

//  SteamAPI_Shutdown();

  app.release(nullptr);

  return 0;
}

GLint GLSetting() {
  glEnable(GL_TEXTURE_2D);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  return RET_SUCC;
}

int32_t JsonAppInit(const char *filename) {
  // Open JsonFile
  rapidjson::Document doc;
  OpenJsonFile(filename, &doc);

  // create window
  assert(doc.HasMember("title"));
  assert(doc.HasMember("windowsize"));
  assert(doc["title"].IsString());
  assert(doc["windowsize"].IsObject());

  SDL::Window window;
  Math::Size window_size = {0, 0};
  JsonParseMethods::ReadSize(doc["windowsize"].GetObject(), &window_size);
  window.create(window_size.w, window_size.h, doc["title"].GetString());

  // Initialize application
  Application &app = Application::Get();
  if (RET_FAILED == app.create(window, GLSetting))
    return RET_FAILED;

  // alloc memories
  assert(doc.HasMember("allocator_size"));
  assert(doc.HasMember("shaderstorage_size"));
  assert(doc.HasMember("texturestorage_size"));
  assert(doc.HasMember("animationstorage_size"));
  assert(doc.HasMember("chunkstorage_size"));
  assert(doc["allocator_size"].IsUint());
  assert(doc["shaderstorage_size"].IsUint());
  assert(doc["texturestorage_size"].IsUint());
  assert(doc["animationstorage_size"].IsUint());
  assert(doc["chunkstorage_size"].IsUint());

  StackAllocator &allocator = app.getAllocator();
  auto &shaders = app.getShaderStorage();
  auto &textures = app.getTextureStorage();
  auto &animations = app.getAnimationStorage();
  auto &chunks = app.getChunkStorage();
  uint32_t shaders_size = doc["shaderstorage_size"].GetUint();
  uint32_t textures_size = doc["texturestorage_size"].GetUint();
  uint32_t animations_size = doc["animationstorage_size"].GetUint();
  uint32_t chunks_size = doc["chunkstorage_size"].GetUint();

  allocator.reallocBuffer(doc["allocator_size"].GetUint());
  shaders.assignMemory(allocator.alloc<Storage<ShaderProgram>::Type>(shaders_size), shaders_size);
  textures.assignMemory(allocator.alloc<Storage<Texture>::Type>(textures_size), textures_size);
  animations.assignMemory(allocator.alloc<Storage<Animation>::Type>(animations_size), animations_size);
  chunks.assignMemory(allocator.alloc<Storage<Mix_Chunk>::Type>(chunks_size), chunks_size);

  // add factory funcs
  AddFactoryFuncs();

  // set scene
  assert(doc.HasMember("default_scene"));
  assert(doc["default_scene"].IsString());
  if (RET_FAILED == app.setScene(doc["default_scene"].GetString()))
    return RET_FAILED;

  return RET_SUCC;
}

void AddFactoryFuncs() {
  auto &allocator = Application::Get().getAllocator();

  // component factory
  auto &component_factory = ComponentFactory::getGlobal();
  component_factory.assignMemory(allocator.alloc<ComponentFactory::Type>(4096), 4096);
  component_factory.addFunction<Collider>(Collider::Factory);
  component_factory.addFunction<CircleCollider>();
  component_factory.addFunction<BoxCollider>();
  component_factory.addFunction<Rigidbody>();
  component_factory.addFunction<Component>(Component::Factory);
  component_factory.addFunction<Drawable>(Drawable::Factory);
  component_factory.addFunction<Sprite>();
  component_factory.addFunction<AnimatedSprite>();
  component_factory.addFunction<Game::Alpaca::Alpaca>();
  component_factory.addFunction<Game::Alpaca::Head>();
  component_factory.addFunction<Game::Ground>();
  component_factory.addFunction<Game::HitGauge>();
  component_factory.addFunction<Game::Alpaca::Spit>();
  component_factory.addFunction<Game::GameManager>();
  component_factory.addFunction<Game::CameraController>();
  component_factory.addFunction<Game::NumberViewer>();
  component_factory.addFunction<Game::HitGaugeViewer>();
  component_factory.addFunction<Game::OutsidedEffect>();
  component_factory.addFunction<Game::LifeViewer>();
  component_factory.addFunction<Game::CooltimeViewer>();
  component_factory.addFunction<Game::Button>();
  component_factory.addFunction<Game::RestartButton>();
  component_factory.addFunction<Game::ResumeButton>();
  component_factory.addFunction<Game::SceneTransitionButton>();
  component_factory.addFunction<Game::BannerMover>();
  component_factory.addFunction<Game::WinnerBanner>();
  component_factory.addFunction<Title::TitleBanner>();
  component_factory.addFunction<Title::TutorialBanner>();
  component_factory.addFunction<Title::Rotator>();
  component_factory.addFunction<Title::ScaleRepeater>();
  component_factory.addFunction<Title::ZoomTransition>();
  component_factory.addFunction<Game::Alpaca::WalkSound>();

  // action factory
  auto &action_factory = Game::Alpaca::ActionFactory::getGlobal();
  action_factory.assignMemory(allocator.alloc<Game::Alpaca::ActionFactory::Type>(128), 128);
  action_factory.addFunction<Game::Alpaca::Heading>();
  action_factory.addFunction<Game::Alpaca::Dash>();
  action_factory.addFunction<Game::Alpaca::Spitting>();
}
