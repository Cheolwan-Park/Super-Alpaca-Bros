#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include <algorithm>
#include <functional>

namespace Base {
// Scene class
Scene::Scene()
    : m_object_storages(4), m_drawable_storages(4), m_cameras(4),
      m_physics(), m_doc() {
  ;
}

Scene::~Scene() {
  for (size_t i = 0; i < m_object_storages.getCount(); ++i) {
    m_object_storages[i]->~ObjectStorage();
    m_object_storages[i] = nullptr;
  }
  for (size_t i = 0; i < m_drawable_storages.getCount(); ++i) {
    m_drawable_storages[i]->~DrawableStorage();
    m_drawable_storages[i] = nullptr;
  }
}

int32 Scene::init() {
#ifndef NDEBUG
  printf("==== loading scene ====\n");
#endif

  if (RET_SUCC != loadPhysics())
    return RET_FAILED;

  if (RET_SUCC != loadCameras())
    return RET_FAILED;

  if (RET_SUCC != loadShaders())
    return RET_FAILED;

  if (RET_SUCC != loadTextures())
    return RET_FAILED;

  if (RET_SUCC != loadAnimations())
    return RET_FAILED;

  if (RET_SUCC != loadObjectStorages())
    return RET_FAILED;

  if (RET_SUCC != loadDrawableStorages())
    return RET_FAILED;

  if (RET_SUCC != loadGameObjects())
    return RET_FAILED;

  // m_doc.Clear();

  return RET_SUCC;
}

GameObject *Scene::getObject(Uint32 hash) {
  GameObject *obj = nullptr;
  for (size_t i = 0; i < m_object_storages.getCount(); ++i) {
    obj = m_object_storages[i]->get(hash);
    if (obj)
      return obj;
  }
  return nullptr;
}

GameObject *Scene::getObject(Uint32 storage, Uint32 hash) {
  for (size_t i = 0; i < m_object_storages.getCount(); ++i) {
    if (m_object_storages[i]->getID() == storage) {
      return m_object_storages[i]->get(hash);
    }
  }
  return nullptr;
}

void Scene::update() {
  m_physics.update();
  for (size_t i = 0; i < m_object_storages.getCount(); ++i) {
    m_object_storages[i]->updateObjects();
  }
}

void Scene::render() {
  for (size_t i = 0; i < m_drawable_storages.getCount(); ++i) {
    m_drawable_storages[i]->drawDrawables();
  }
}

void Scene::checkDeletedObjects() {
  m_physics.checkDeleted();
  for (size_t i = 0; i < m_drawable_storages.getCount(); ++i) {
    m_drawable_storages[i]->checkDeleted();
  }
  for (size_t i = 0; i < m_object_storages.getCount(); ++i) {
    m_object_storages[i]->checkAndDelete();
  }
}

void Scene::addObjectStorage(ObjectStorage *storage) {
  assert(storage);
  m_object_storages.append(storage);
  auto *front = m_object_storages.getBuffer();
  auto *end = front + m_object_storages.getCount();
  std::sort(front, end,
            [](ObjectStorage *a, ObjectStorage *b) {
              return (a->getOrder() > b->getOrder());
            });
}

void Scene::addDrawableStorage(DrawableStorage *storage) {
  assert(storage);
  m_drawable_storages.append(storage);
  auto *front = m_drawable_storages.getBuffer();
  auto *end = front + m_drawable_storages.getCount();
  std::sort(front, end,
            [](DrawableStorage *a, DrawableStorage *b) {
              return (a->getOrder()) > (b->getOrder());
            });
}

ObjectStorage *Scene::getObjectStorage(Uint32 hash) {
  for (size_t i = 0; i < m_object_storages.getCount(); ++i) {
    if (hash == m_object_storages[i]->getID())
      return m_object_storages[i];
  }
  return nullptr;
}

DrawableStorage *Scene::getDrawableStorage(Uint32 hash) {
  for (size_t i = 0; i < m_drawable_storages.getCount(); ++i) {
    if (hash == m_drawable_storages[i]->getID())
      return m_drawable_storages[i];
  }
  return nullptr;
}

Camera *Scene::getCamera(Uint32 hash) {
  for (size_t i = 0; i < m_cameras.getCount(); ++i) {
    if (hash == m_cameras[i]->getID())
      return m_cameras[i];
  }
  return nullptr;
}

Physics &Scene::getPhysics() {
  return m_physics;
}

GameObject *Scene::addGameObject(Uint32 storage, GameObject *gameobject) {
  assert(gameobject);
  for (size_t i = 0; i < m_object_storages.getCount(); ++i) {
    if (storage == m_object_storages[i]->getID()) {
      m_object_storages[i]->add(gameobject);
      return gameobject;
    }
  }
  return nullptr;
}

int32 Scene::addDrawable(Uint32 storage, Drawable *drawable) {
  assert(drawable);
  for (size_t i = 0; i < m_drawable_storages.getCount(); ++i) {
    if (storage == m_drawable_storages[i]->getID()) {
      m_drawable_storages[i]->add(drawable);
      return RET_SUCC;
    }
  }
  return RET_FAILED;
}
}














