#ifndef BaseAnimation_hpp
#define BaseAnimation_hpp

#include "types.hpp"
#include "Math.hpp"
#include "Json.hpp"

namespace Base {
class Animation {
 public:
  Animation();

  Animation(const Animation &other);

  ~Animation();

  Animation &operator=(const Animation &other);

  void initWithJson(const rapidjson::Value::Object &obj);

  // get
  Uint32 getID() const;

  const Math::IRect &getSpritesRange() const;

  const Math::Size &getSpritesCount() const;

  Uint32 getTextureID() const;

  float32 getEachTime() const;

  int32 isLooping() const;

 private:
  Uint32 m_id;
  Math::IRect m_sprites_range;
  Math::Size m_sprites_count;
  Uint32 m_texture_id;
  float32 m_each_time;
  int32 m_looping;
};

void FreeAnimation(Animation **anim);
}

#endif 