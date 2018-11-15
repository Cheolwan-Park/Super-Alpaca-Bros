#ifndef BaseScene_hpp
#define BaseScene_hpp

#include "Application.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Allocator.hpp"
#include "types.hpp"
#include "Array.hpp"
#include "Json.hpp"
#include "FileIO.hpp"
#include "Physics.hpp"
#include "Camera.hpp"

namespace Base {
class Scene {
 public:
  Scene();

  Scene(const Scene &other) = delete;

  virtual ~Scene();

  Scene &operator=(const Scene &other) = delete;

  virtual int32 init();

  virtual GameObject *getObject(Uint32 hash);

  virtual GameObject *getObject(Uint32 storage, Uint32 hash);

  virtual void update();

  virtual void render();

  virtual void checkDeletedObjects();

  void addObjectStorage(ObjectStorage *storage);

  void addDrawableStorage(DrawableStorage *storage);

  ObjectStorage *getObjectStorage(Uint32 hash);

  DrawableStorage *getDrawableStorage(Uint32 hash);

  Camera *getCamera(Uint32 hash);

  Physics &getPhysics();

  GameObject *addGameObject(Uint32 storagehash, GameObject *gameobject);

  int32 addDrawable(Uint32 storagehash, Drawable *drawable);

  void setJsonFile(const FileIO &f);

  int32 setJsonFile(const char *filename);

  GameObject *createGameObject(const rapidjson::Value::Object &obj);

 protected:
  virtual int32 loadPhysics();
  virtual int32 loadCameras();
  virtual int32 loadShaders();
  virtual int32 loadTextures();
  virtual int32 loadAnimations();
  virtual int32 loadSounds();
  virtual int32 loadObjectStorages();
  virtual int32 loadDrawableStorages();
  virtual int32 loadGameObjects();

 private:
  Array<ObjectStorage *> m_object_storages;
  Array<DrawableStorage *> m_drawable_storages;
  Array<Camera *> m_cameras;
  Physics m_physics;
  rapidjson::Document m_doc;
};
}

#endif
