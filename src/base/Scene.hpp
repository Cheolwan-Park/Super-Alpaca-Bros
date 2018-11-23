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

  virtual int32_t init();

  virtual GameObject *getObject(uint32_t hash);

  virtual GameObject *getObject(uint32_t storage, uint32_t hash);

  virtual void update();

  virtual void render();

  virtual void checkDeletedObjects();

  void addObjectStorage(ObjectStorage *storage);

  void addDrawableStorage(DrawableStorage *storage);

  ObjectStorage *getObjectStorage(uint32_t hash);

  DrawableStorage *getDrawableStorage(uint32_t hash);

  Camera *getCamera(uint32_t hash);

  Physics &getPhysics();

  GameObject *addGameObject(uint32_t storagehash, GameObject *gameobject);

  int32_t addDrawable(uint32_t storagehash, Drawable *drawable);

  void setJsonFile(const FileIO &f);

  int32_t setJsonFile(const char *filename);

  GameObject *createGameObject(const rapidjson::Value::Object &obj);

 protected:
  virtual int32_t loadPhysics();
  virtual int32_t loadCameras();
  virtual int32_t loadShaders();
  virtual int32_t loadTextures();
  virtual int32_t loadAnimations();
  virtual int32_t loadSounds();
  virtual int32_t loadObjectStorages();
  virtual int32_t loadDrawableStorages();
  virtual int32_t loadGameObjects();

 private:
  Array<ObjectStorage *> m_object_storages;
  Array<DrawableStorage *> m_drawable_storages;
  Array<Camera *> m_cameras;
  Physics m_physics;
  rapidjson::Document m_doc;
};
}

#endif
