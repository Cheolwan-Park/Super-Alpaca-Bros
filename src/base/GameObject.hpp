#ifndef BaseGameObject_hpp
#define BaseGameObject_hpp

#include "types.hpp"
#include "Math.hpp"
#include "Array.hpp"
#include "String.hpp"
#include "BitFlag.hpp"
#include "Storage.hpp"
#include "Json.hpp"
#include "Component.hpp"
#include <glm/glm.hpp>

namespace Base {
class Schedule;
class ObjectStorage;
class Component;
class Collider;

class GameObject {
 public:
  static GameObject *Factory(const ::rapidjson::Value::Object &obj,
                             ::Base::StackAllocator &allocator,
                             ::Base::ObjectStorage *storage,
                             std::uint32_t id);

 public:
  GameObject();

  explicit GameObject(uint32_t id, int32_t isStatic = false);

  explicit GameObject(uint32_t id, const GameObject *parent, int32_t isStatic = false);

  // not copying parent, id, isDeleted, isIdSet, isStarted, components
  GameObject(const GameObject &other);

  ~GameObject();

  // not copying parent, id, isDeleted, isIdSet, isStarted, components
  GameObject &operator=(const GameObject &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

  void start();

  void update();

  void release();

  GameObject *addChild(GameObject *child) const;

  // collide callback
  void onColliderEnter(Collider *other);
  void onColliderStay(Collider *other);
  void onColliderExit(Collider *other);

  // trigger callback
  void onTriggerEnter(Collider *other);
  void onTriggerStay(Collider *other);
  void onTriggerExit(Collider *other);

  // get
  uint32_t getID() const;

  uint32_t getTag() const;

  const glm::vec3 &getLocalPosition() const;

  void getWorldPosition(glm::vec3 *vec) const;

  const GameObject *getParent() const;

  int32_t isDeleted() const;

  int32_t isAvailable() const;

  int32_t isStarted() const;

  int32_t isStatic() const;

  const glm::vec2 &getScale() const;

  float32 getRotation() const;

  const glm::mat3x3 &getLocalModel() const;

  void getModel(glm::mat3x3 *mat) const;

  template<class T>
  T *getComponent() {
    for (uint32_t i = 0; i < m_component_count; ++i) {
      if (T::ID == m_components[i]->getTypeID())
        return static_cast<T *>(m_components[i]);
    }
    return nullptr;
  }

  template<class T>
  const T *getComponent() const {
    for (uint32_t i = 0; i < m_component_count; ++i) {
      if (T::ID == m_components[i]->getTypeID())
        return static_cast<T *>(m_components[i]);
    }
    return nullptr;
  }

  uint32_t getComponentCount() const;

  // set
  void setID(uint32_t id);

  void setTag(uint32_t tag);

  void setLocalPosition(const glm::vec3 &position);

  void setLocalPosition(float32 x, float32 y);

  void setLocalPosition(float32 x, float32 y, float32 z);

  void setWorldPosition(const glm::vec3 &position);

  void setWorldPosition(float32 x, float32 y);

  void setWorldPosition(float32 x, float32 y, float32 z);

  void move(const glm::vec3 &delta);

  void move(float32 x, float32 y);

  void setScale(const glm::vec2 &val);

  void setScale(float32 x, float32 y);

  void scale(const glm::vec2 &val);

  void scale(float32 x, float32 y);

  void scale(float32 x);

  void setRotation(float32 val);

  void rotate(float32 delta);

  void setParent(const GameObject *parent);

  void del();

  void setAvailable(int32_t value);

 private:
  uint32_t m_id;
  uint32_t m_tag;
  uint32_t m_component_count;
  float32 m_rotation;
  Component **m_components;
  const GameObject *m_parent;
  glm::vec3 m_position;
  glm::vec2 m_scale;
  glm::mat3x3 m_model;

 protected:
  /* flags
   * 0 : isDeleted
   * 1 : isAvailable (default : true)
   * 2 : isStarted
   * 3 : isStatic
   * 4 : isIdSet
   */
  BitFlag m_flags;
};

class ObjectStorage {
 public:
  typedef GameObject *Type;

  ObjectStorage();

  ObjectStorage(const ObjectStorage &other) = delete;

  ~ObjectStorage();

  ObjectStorage &operator=(const ObjectStorage &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

  GameObject *add(GameObject *gameobject);

  GameObject *get(uint32_t hash);

  void updateObjects();

  void checkAndDelete();

  void clear();

  // get
  uint32_t getID() const;

  uint32_t getOrder() const;

 private:
  uint32_t m_id;
  uint32_t m_order;
  uint32_t m_len;
  Type *m_gameobjects;
};
}

#endif
