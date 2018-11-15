#include "Scene.hpp"
#include "SDLMixer.h"

namespace Base {
void Scene::setJsonFile(const FileIO &f) {
  OpenJsonFile(f, &m_doc);
}

int32 Scene::setJsonFile(const char *filename) {
  String128 path(Directories::Scene);
  path += filename;
  return OpenJsonFile(path.c_str(), &m_doc);
}

GameObject *Scene::createGameObject(const rapidjson::Value::Object &obj) {
  if (obj.HasMember("file")) {
    assert(obj["file"].IsString());
    String128 path("");
    path = Directories::GameObject;
    path += obj["file"].GetString();
    rapidjson::Document doc;
    OpenJsonFile(path.c_str(), &doc);

    assert(doc.HasMember("Object"));
    assert(doc["Object"].IsObject());

    return createGameObject(doc["Object"].GetObject());
  }

  assert(obj.HasMember("storage"));
  assert(obj.HasMember("id"));
  assert(obj["storage"].IsString());
  assert(obj["id"].IsString());

  const char *storage_id = obj["storage"].GetString();
  const char *id = obj["id"].GetString();

  auto *storage = getObjectStorage(CompileTimeHash::runtime_hash(storage_id, strlen(storage_id)));
  assert(storage);
  GameObject *new_gameobject = GameObject::Factory(obj,
                                              Application::Get().getAllocator(),
                                              storage,
                                              CompileTimeHash::runtime_hash(id, strlen(id)));
  assert(new_gameobject);
#ifndef NDEBUG
  printf("%s(%u) added\n", id, new_gameobject->getID());
#endif
  return new_gameobject;
}

int32 Scene::loadPhysics() {
  assert(m_doc.HasMember("Physics"));
  assert(m_doc["Physics"].IsObject());

  m_physics.initWithJson(m_doc["Physics"].GetObject(), Application::Get().getAllocator());

  return RET_SUCC;
}

int32 Scene::loadCameras() {
  Application &app = Application::Get();
  auto &allocator = app.getAllocator();

  assert(m_doc.HasMember("Cameras"));
  assert(m_doc["Cameras"].IsArray());
  auto load_list = m_doc["Cameras"].GetArray();

  Camera *new_camera = nullptr;
  for (auto itr = load_list.begin();
       load_list.end() != itr;
       ++itr) {
    assert(itr->IsObject());
    auto obj = itr->GetObject();

    new_camera = new(allocator.alloc<Camera>()) Camera();
    assert(new_camera);
    new_camera->initWithJson(obj);
    m_cameras.append(new_camera);
#ifndef NDEBUG
    printf("camera : name(%s)\n", obj["name"].GetString());
#endif
  }

  return RET_SUCC;
}

int32 Scene::loadShaders() {
  Application &app = Application::Get();
  auto &shaders = app.getShaderStorage();
  auto &allocator = app.getAllocator();

  ShaderProgram *program = nullptr;

  assert(m_doc.HasMember("Shaders"));
  assert(m_doc["Shaders"].IsArray());
  auto load_list = m_doc["Shaders"].GetArray();

  for (auto itr = load_list.begin();
       load_list.end() != itr;
       ++itr) {
    assert(itr->IsObject());
    auto obj = itr->GetObject();

    program = new(allocator.alloc<ShaderProgram>()) ShaderProgram();
    if (RET_SUCC != program->initWithJson(obj)) {
      return RET_FAILED;
    }
    shaders.add(program, program->getID());

#ifndef NDEBUG
    printf("shader : name(%s) vert(%s) frag(%s)\n",
           obj["name"].GetString(),
           obj["vert"].GetString(),
           obj["frag"].GetString());
#endif
  }

  return RET_SUCC;
}

int32 Scene::loadTextures() {
  Application &app = Application::Get();
  auto &textures = app.getTextureStorage();
  auto &allocator = app.getAllocator();

  Texture *texture = nullptr;

  assert(m_doc.HasMember("Textures"));
  assert(m_doc["Textures"].IsArray());
  auto load_list = m_doc["Textures"].GetArray();

  for (auto itr = load_list.begin();
       load_list.end() != itr;
       ++itr) {
    assert(itr->IsObject());
    auto obj = itr->GetObject();

    texture = new(allocator.alloc<Texture>()) Texture({0, 0, 0});
    LoadTexture(texture, obj);

    textures.add(texture, texture->hash_id);

#ifndef NDEBUG
    printf("tex : %s\n", obj["file"].GetString());
#endif
  }

  return RET_SUCC;
}

int32 Scene::loadAnimations() {
  Application &app = Application::Get();
  auto &allocator = app.getAllocator();
  auto &animations = app.getAnimationStorage();

  assert(m_doc.HasMember("Animations"));
  assert(m_doc["Animations"].IsArray());
  const auto load_list = m_doc["Animations"].GetArray();

  Animation *new_animation = nullptr;
  for (auto itr = load_list.begin();
       load_list.end() != itr;
       ++itr) {
    assert(itr->IsObject());
    auto anim = itr->GetObject();

    new_animation = new(allocator.alloc<Animation>()) Animation();
    assert(new_animation);
    new_animation->initWithJson(anim);

    animations.add(new_animation, new_animation->getID());

#ifndef NDEBUG
    printf("anim : %s\n", anim["name"].GetString());
#endif
  }

  return RET_SUCC;
}

int32 Scene::loadSounds() {
  Application &app = Application::Get();
  auto &chunks = app.getChunkStorage();
  auto &mixer = SDL::Mixer::Get();

  assert(m_doc.HasMember("Sounds"));
  assert(m_doc["Sounds"].IsArray());
  auto load_list = m_doc["Sounds"].GetArray();

  Mix_Chunk *new_chunk;
  for(auto iter = load_list.begin();
      load_list.end() != iter;
      ++iter) {
    assert(iter->IsObject());
    auto obj = iter->GetObject();

    assert(obj.HasMember("type"));
    assert(obj.HasMember("file"));
    assert(obj.HasMember("volume"));
    assert(obj["type"].IsString());
    assert(obj["file"].IsString());
    assert(obj["volume"].IsUint());

    const char *str_type = obj["type"].GetString();
    StringID type_id(str_type);
    const char *filename = obj["file"].GetString();
    Uint32 volume = obj["volume"].GetUint();

    if("Music"_hash == (Uint32)type_id) {
      mixer.changeMusic(filename);
      mixer.setMusicVolume(volume);
#ifndef NDEBUG
      printf("music : %s\n", filename);
#endif
    } else if("Chunk"_hash == (Uint32)type_id) {
      new_chunk = mixer.loadChunk(filename);
      mixer.setVolume(new_chunk, volume);
      StringID filename_id(filename);
      chunks.add(new_chunk, (Uint32)filename_id);
#ifndef NDEBUG
      printf("chunk : %s\n", filename);
#endif
    } else {
      fprintf(stderr, "Invalid Sound Type\n");
      return false;
    }
  }

  return RET_SUCC;
}


int32 Scene::loadObjectStorages() {
  Application &app = Application::Get();
  auto &allocator = app.getAllocator();

  assert(m_doc.HasMember("ObjectStorages"));
  assert(m_doc["ObjectStorages"].IsArray());
  auto load_list = m_doc["ObjectStorages"].GetArray();

  ObjectStorage *new_storage = nullptr;
  for (auto itr = load_list.begin();
       load_list.end() != itr;
       ++itr) {
    assert(itr->IsObject());
    auto obj = itr->GetObject();

    new_storage = new(allocator.alloc<ObjectStorage>()) ObjectStorage();
    new_storage->initWithJson(obj, allocator);
    addObjectStorage(new_storage);

#ifndef NDEBUG
    printf("objsto : name(%s) size(%d) order(%d)\n",
           obj["name"].GetString(),
           obj["size"].GetInt(),
           obj["order"].GetInt());
#endif
  }

  return RET_SUCC;
}

int32 Scene::loadDrawableStorages() {
  Application &app = Application::Get();
  StackAllocator &allocator = app.getAllocator();

  assert(m_doc.HasMember("DrawableStorages"));
  assert(m_doc["DrawableStorages"].IsArray());
  auto load_list = m_doc["DrawableStorages"].GetArray();

  DrawableStorage *new_storage = nullptr;

  for (auto itr = load_list.begin();
       load_list.end() != itr;
       ++itr) {
    assert(itr->IsObject());
    auto obj = itr->GetObject();
    new_storage = new(allocator.alloc<DrawableStorage>()) DrawableStorage();
    new_storage->initWithJson(obj, allocator);
    addDrawableStorage(new_storage);

#ifndef NDEBUG
    printf("drawablesto : name(%s) shader(%s) size(%d) order(%d)\n",
           obj["name"].GetString(),
           obj["shader"].GetString(),
           obj["size"].GetInt(),
           obj["order"].GetInt());
#endif
  }

  return RET_SUCC;
}

int32 Scene::loadGameObjects() {
  Application &app = Application::Get();
  auto &allocator = app.getAllocator();

  assert(m_doc.HasMember("GameObjects"));
  assert(m_doc["GameObjects"].IsArray());
  auto load_list = m_doc["GameObjects"].GetArray();

  for (auto itr = load_list.begin();
       load_list.end() != itr;
       ++itr) {
    assert(itr->IsObject());
    createGameObject(itr->GetObject());
  }

  return RET_SUCC;
}
}