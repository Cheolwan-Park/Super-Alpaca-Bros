#ifndef BaseComponent_hpp
#define BaseComponent_hpp

#include "Json.hpp"
#include "BitFlag.hpp"
#include "Storage.hpp"
#include "Allocator.hpp"
#include "String.hpp"
#include <glm/glm.hpp>

#ifndef MAKE_TYPE_ID
#define MAKE_TYPE_ID(__TYPENAME__)  \
static constexpr uint32_t ID = ::Base::CompileTimeHash::fnv1a_32(#__TYPENAME__, sizeof(#__TYPENAME__)-1);
#endif

#ifndef COMPONENT
#define COMPONENT(__TYPENAME__)     \
MAKE_TYPE_ID(__TYPENAME__);         \
uint32_t getTypeID()const override  { return ID; }
#endif

namespace Base {
class GameObject;
class Collider;

class Component {
 public:
  typedef Component
      *(FactoryFunc)(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator, GameObject *gameobject);

  static Component *Factory(const ::rapidjson::Value::Object &obj,
                            ::Base::StackAllocator &allocator,
                            GameObject *gameobject);

 public:
  MAKE_TYPE_ID(Component);
  virtual uint32_t getTypeID()const { return ID; }

  Component();

  // not copying gameobject, isStarted
  Component(const Component &other);

  virtual ~Component();

  // not copying gameobject, isStarted
  Component &operator=(const Component &other);

  virtual void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

  virtual void start();

  virtual void update();

  virtual void release() = 0;

  // collide callback
  virtual void onColliderEnter(Collider *other);
  virtual void onColliderStay(Collider *other);
  virtual void onColliderExit(Collider *other);

  // trigger callback
  virtual void onTriggerEnter(Collider *other);
  virtual void onTriggerStay(Collider *other);
  virtual void onTriggerExit(Collider *other);

  // get
  GameObject *getGameObject();

  const GameObject *getGameObject()const;

  int32_t isAvailable() const;

  int32_t isStarted() const;

  // set
  void setGameObject(GameObject *gameobject);

  void setAvailable(int32_t val);

  // gameobject functions wrapping
  uint32_t getTag() const;

  const glm::vec3 &getLocalPosition() const;

  void getWorldPosition(glm::vec3 *vec) const;

  const GameObject *getParent() const;

  const glm::vec2 &getScale() const;

  float32 getRotation() const;

  const glm::mat3x3 &getLocalModel() const;

  void getModel(glm::mat3x3 *mat) const;

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

 private:
  GameObject *m_gameobject;

 protected:
  BitFlag m_flags;
  /*
   * flags
   * 0 : isAvailable (default : true)
   * 1 : isStarted
   */
};

template<class T>
Component *ComponentDefaultFactory(const ::rapidjson::Value::Object &obj,
                                   ::Base::StackAllocator &allocator,
                                   GameObject *gameobject) {
  assert(gameobject);
  Component *result = new(allocator.alloc<T>()) T();
  assert(result);
  result->setGameObject(gameobject);
  result->initWithJson(obj, allocator);
  return result;
}

class ComponentFactory : public Storage<Component::FactoryFunc> {
 public:
  ComponentFactory() : Storage<Component::FactoryFunc>() { ; }

  ComponentFactory(const ComponentFactory &other) = delete;

  ~ComponentFactory() { clear(); }

  ComponentFactory &operator=(const ComponentFactory) = delete;

  template<typename T>
  void addFunction() {
    add(&ComponentDefaultFactory<T>, T::ID);
  }

  template<typename T>
  void addFunction(Component::FactoryFunc func) {
    add(func, T::ID);
  }

  template<typename T>
  Component::FactoryFunc *getFunction() {
    return (get(T::ID));
  }

  Component::FactoryFunc *getFunction(uint32_t id) {
    return (get(id));
  }

  static ComponentFactory &getGlobal() {
    static ComponentFactory instance;
    return instance;
  }
};

}

#endif