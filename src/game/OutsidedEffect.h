#ifndef ALPACAS_OUTSIDEDEFFECT_H
#define ALPACAS_OUTSIDEDEFFECT_H

#include "base.hpp"

using namespace Base;

namespace Game {
class OutsidedEffect : public Component {
 public:
  COMPONENT(OutsidedEffect);

  OutsidedEffect();

  OutsidedEffect(const OutsidedEffect &other) = delete;

  ~OutsidedEffect() override;

  OutsidedEffect &operator=(const OutsidedEffect &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void generate(const glm::vec2 &direction);

  void setAlpacaIndex(Uint32 index);

 private:
  void generate();

  int32 isGenerating();

  void setGenerating(int32 val);

 private:
  struct Particle {
    float32 velocity;
    float32 remain_time;
  };

 private:
  Uint32 m_alpaca_index;
  Uint32 m_particle_count;
  Uint32 m_max_particle_count;
  Uint32 m_uv_count;

  float32 m_radius;
  float32 m_generate_speed;
  float32 m_life_time;
  float32 m_scale;
  glm::vec2 m_velocity_range;
  glm::vec3 m_direction;

  Particle *m_particles;
  Sprite **m_particle_sprites;
  Math::Rect *m_uvs;

  /*
   * flags
   * Drawable's flags
   * 10 : isGenerating
   */
};
}

#endif //ALPACAS_OUTSIDEDEFFECT_H
