#ifndef GameAlpacaSpit_hpp
#define GameAlpacaSpit_hpp

#include "base.hpp"

using namespace Base;

namespace Game {
namespace Alpaca {
class Alpaca;

class Spit : public Component {
 public:
  COMPONENT(Spit);

  Spit();

  Spit(const Spit &other);

  ~Spit() override;

  Spit &operator=(const Spit &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void onTriggerEnter(Collider *other) override;

  void shoot(const glm::vec3 &direction, Alpaca *owner);

  int32 isShot() const;

 private:
  float32 m_elapsed_time;
  float32 m_lifetime;
  float32 m_speed;
  float32 m_force;
  float32 m_gauge_up;
  Alpaca *m_owner;
  glm::vec3 m_direction;

  /*
   * flags
   * Component's flags
   * 2 : isShot
   */
};
}
}

#endif