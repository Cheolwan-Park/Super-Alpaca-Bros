#ifndef BasePhysics_hpp
#define BasePhysics_hpp

#include "Json.hpp"
#include "Component.hpp"

namespace Base {
class Collider;

class Physics {
 public:
  Physics();

  Physics(const Physics &other) = delete;

  ~Physics();

  Physics &operator=(const Physics &other) = delete;

  virtual void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

  void update();

  void checkDeleted();

  Collider *add(Collider *collider);

  float32 getGravity() const;

  void setGravity(float32 a);

  uint32_t getColliderCount() const;

  uint32_t getMaxColliderCount() const;

 private:
  float32 m_gravity;
  uint32_t m_collider_count;
  uint32_t m_max_collider_count;
  Collider **m_colliders;
  int8_t *m_collide_map;
};

class Rigidbody : public Component {
 public:
  COMPONENT(Rigidbody);

  Rigidbody();

  Rigidbody(const Rigidbody &other);

  ~Rigidbody() override;

  Rigidbody &operator=(const Rigidbody &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void onColliderEnter(Collider *other) override;

  void accelerate(const glm::vec3 &a);

  void accelerate(float32 x, float32 y, float32 z);

  void addForce(const glm::vec3 &f);

  void addForce(float32 x, float32 y, float32 z);

  // get
  float32 getMass() const;

  const glm::vec3 &getVelocity() const;

  int32_t isSimulated() const;

  int32_t isGravityActive() const;

  // set
  void setMass(float32 m);

  void setVelocity(const glm::vec3 &v);

  void setVelocity(float32 x, float32 y, float32 z);

  void setSimulated(int32_t val);

  void setGravityActive(int32_t val);

 private:
  float32 m_mass;
  float32 m_drag;
  glm::vec3 m_velocity;

  /*
   * flags
   * Component's flags
   * 2 : isSimulated
   * 3 : gravity active
   */
};
}

#endif