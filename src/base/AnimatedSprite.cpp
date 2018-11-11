#include "AnimatedSprite.hpp"
#include "Time.hpp"
#include "Application.hpp"

namespace Base {
// AnimatedSprite class
AnimatedSprite::AnimatedSprite()
    : Sprite(), m_change_remain_time(0.0f), m_now_idx(0), m_animation(nullptr) {
  ;
}

AnimatedSprite::AnimatedSprite(const AnimatedSprite &other)
    : Sprite(other), m_change_remain_time(other.m_change_remain_time), m_now_idx(other.m_now_idx),
      m_animation(other.m_animation) {
  ;
}

AnimatedSprite::~AnimatedSprite() {
  ;
}

AnimatedSprite &AnimatedSprite::operator=(const AnimatedSprite &other) {
  assert(this != &other);
  Sprite::operator=(other);
  this->m_change_remain_time = other.m_change_remain_time;
  this->m_now_idx = other.m_now_idx;
  this->m_animation = other.m_animation;
  return (*this);
}

void AnimatedSprite::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Sprite::initWithJson(obj, allocator);

  assert(obj.HasMember("default_anim"));
  assert(obj["default_anim"].IsString());
  const char *default_anim = obj["default_anim"].GetString();

  auto &animations = Application::Get().getAnimationStorage();
  setAnimation(animations[CompileTimeHash::runtime_hash(default_anim, strlen(default_anim))]);
}

void AnimatedSprite::start() {
  Sprite::start();
}

void AnimatedSprite::update() {
  Sprite::update();
  if (m_animation && isPlaying()) {
    Time &t = Time::Get();
    m_change_remain_time -= t.getDeltatime();
    if (m_change_remain_time < 0.0f) {
      ++m_now_idx;

      const Math::IRect &sprites_range = m_animation->getSpritesRange();
      const Math::Size &sprites_count = m_animation->getSpritesCount();

      if (m_now_idx >= sprites_count.w * sprites_count.h) {
        if (m_animation->isLooping()) {
          m_now_idx = 0;
          setAnimationUV();
        } else {
          m_flags.setFlag(12, false);
          onFinish();
        }
      } else
        setAnimationUV();
      m_change_remain_time = m_animation->getEachTime();
    }
  }
}

void AnimatedSprite::release() {
  Sprite::release();
}

int32 AnimatedSprite::isPlaying() const {
  return m_flags.getFlag(10);
}

const Animation *AnimatedSprite::getAnimation() const {
  return m_animation;
}

void AnimatedSprite::setPlaying(int32 val) {
  m_flags.setFlag(10, val);
}

void AnimatedSprite::setAnimation(Animation *anim) {
  assert(anim);
  m_animation = anim;
  auto &textures = Application::Get().getTextureStorage();
  setTexture(textures[m_animation->getTextureID()]);
  setPlaying(true);
}

void AnimatedSprite::onFinish() {
  ;
}

void AnimatedSprite::setAnimationUV() {
  const Math::IRect &sprites_range = m_animation->getSpritesRange();
  const Math::Size &sprites_count = m_animation->getSpritesCount();
  int32 x = 0, y = 0;
  Math::IRect uv = {0, 0, 0, 0};
  y = m_now_idx / sprites_count.w;
  x = m_now_idx % sprites_count.w;
  uv.w = sprites_range.w / sprites_count.w;
  uv.h = sprites_range.h / sprites_count.h;
  uv.x = sprites_range.x + x * uv.w;
  uv.y = sprites_range.y + (sprites_count.h-y-1) * uv.h;
  setUV(uv);
}
}