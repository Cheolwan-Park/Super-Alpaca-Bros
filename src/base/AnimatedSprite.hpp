#ifndef BaseAnimatedSprite_hpp
#define BaseAnimatedSprite_hpp

#include "Sprite.hpp"
#include "Math.hpp"
#include "Animation.hpp"

namespace Base {
class AnimatedSprite : public Sprite {
 public:
  COMPONENT(AnimatedSprite);

  AnimatedSprite();

  AnimatedSprite(const AnimatedSprite &other);

  ~AnimatedSprite() override;

  AnimatedSprite &operator=(const AnimatedSprite &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  // get
  int32 isPlaying() const;

  const Animation *getAnimation() const;

  // set
  void setPlaying(int32 val);

  void setAnimation(Animation *anim);

 private:
  virtual void onFinish();
  void setAnimationUV();

 private:
  float32 m_change_remain_time;
  int32 m_now_idx;
  const Animation *m_animation;
  /* flags
   * Sprite's flags
   * 10 : playing animation
   */
};
}

#endif