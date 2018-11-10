#ifndef BaseCollider_hpp
#define BaseCollider_hpp

#include "GameObject.hpp"

namespace Base {
class Rigidbody;

class Collider : public Component {
 public:
  enum class Type : Uint32 { NONE = 0, CIRCLE = 1, BOX = 2 };

  static Component *Factory(const ::rapidjson::Value::Object &obj,
                            ::Base::StackAllocator &allocator,
                            GameObject *gameobject);

 public:
  COMPONENT(Collider);

  Collider();

  Collider(const Collider &other);

  ~Collider() override;

  Collider &operator=(const Collider &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  virtual int32 isCollideWith(const Collider *other) const = 0;

  virtual Type getType() const = 0;

  Rigidbody *getRigidbody();

  const Rigidbody *getRigidbody() const;

  int32 isTrigger() const;

  void setTrigger(int32 val);

 private:
  Rigidbody *m_rigidbody;

  /*
   * flags
   * Component's flags
   * 2 : isTrigger
   */
};

class CircleCollider : public Collider {
 public:
  COMPONENT(CircleCollider);

  CircleCollider();

  CircleCollider(const CircleCollider &other);

  ~CircleCollider() override;

  CircleCollider &operator=(const CircleCollider &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  int32 isCollideWith(const Collider *other) const override;

  Collider::Type getType() const override;

  // get
  float32 getRadius() const;

  // set
  void setRadius(float32 r);

 private:
  float32 m_radius;
};

class BoxCollider : public Collider {
 public:
  COMPONENT(BoxCollider);

  BoxCollider();

  BoxCollider(const BoxCollider &other);

  ~BoxCollider() override;

  BoxCollider &operator=(const BoxCollider &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  int32 isCollideWith(const Collider *other) const override;

  Collider::Type getType() const override;

  // get
  const Math::Rect &getBox() const;

  // set
  void setBox(const Math::Rect &box);

 private:
  Math::Rect m_box;
};

namespace CollideCheckFunctions {
int32 isCollide(const CircleCollider *circle0, const CircleCollider *circle1);
int32 isCollide(const BoxCollider *box, const CircleCollider *circle);
int32 isCollide(const BoxCollider *box0, const BoxCollider *box1);
}
}

#endif