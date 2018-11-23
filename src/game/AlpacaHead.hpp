#ifndef GameHead_hpp
#define GameHead_hpp

#include "base.hpp"

using namespace Base;

namespace Game {
namespace Alpaca {
class Alpaca;

class Head : public Sprite {
 public:
  static constexpr uint32_t NECK_COUNT = 18;

 public:
  COMPONENT(Head);

  Head();

  Head(const Head &other) = delete;

  ~Head() override;

  Head &operator=(const Head &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void onTriggerEnter(Collider *other) override;

  void draw() override;

  void resetPosition();

  // set
  void setAlpaca(Alpaca *alpaca);

  void setHeadPos(const glm::vec3 &pos);

  // get
  GameObject &getNeck(int32_t i);

  size_t getNeckCount() const;

  float32 getForce() const;

  float32 getGaugeUp() const;

  const glm::vec3 &getHeadPos() const;

  const glm::vec3 &getNeckPos() const;

 private:
  Alpaca *m_alpaca;
  float32 m_force;
  glm::vec2 m_force_ratio;
  float32 m_gauge_up;
  FixedArray<GameObject, NECK_COUNT> m_necks;
  FixedArray<Sprite *, NECK_COUNT> m_neck_sprites;
  glm::vec3 m_head_position;
  glm::vec3 m_neck_position;
  /* flags
  * Sprite's flags
  */
};
}
}

#endif