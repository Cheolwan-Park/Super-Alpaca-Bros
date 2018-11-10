#include "Animation.hpp"

namespace Base {
// Animation class

Animation::Animation()
    : m_id(0), m_sprites_range({0, 0, 0, 0}), m_sprites_count({0, 0}),
      m_texture_id(0), m_each_time(0.0f), m_looping(false) {
  ;
}

Animation::Animation(const Animation &other)
    : m_sprites_range(other.m_sprites_range),
      m_sprites_count(other.m_sprites_count),
      m_texture_id(other.m_texture_id),
      m_each_time(other.m_each_time),
      m_looping(other.m_looping) {
  ;
}

Animation::~Animation() {
  ;
}

Animation &Animation::operator=(const Animation &other) {
  assert(this != &other);
  this->m_sprites_range = other.m_sprites_range;
  this->m_sprites_count = other.m_sprites_count;
  this->m_texture_id = other.m_texture_id;
  this->m_each_time = other.m_each_time;
  this->m_looping = other.m_looping;
  return (*this);
}

void Animation::initWithJson(const rapidjson::Value::Object &obj) {
  assert(obj.HasMember("name"));
  assert(obj.HasMember("texture"));
  assert(obj.HasMember("range"));
  assert(obj.HasMember("count"));
  assert(obj.HasMember("eachtime"));
  assert(obj.HasMember("loop"));
  assert(obj["name"].IsString());
  assert(obj["texture"].IsString());
  assert(obj["range"].IsObject());
  assert(obj["count"].IsObject());
  assert(obj["eachtime"].IsFloat());
  assert(obj["loop"].IsBool());

  const char *name = obj["name"].GetString();
  const char *texture_name = obj["texture"].GetString();
  JsonParseMethods::ReadIRect(obj["range"].GetObject(), &m_sprites_range);
  JsonParseMethods::ReadSize(obj["count"].GetObject(), &m_sprites_count);
  m_each_time = obj["eachtime"].GetFloat();
  m_looping = obj["loop"].GetBool();

  StringID id(name), texture_id(texture_name);
  m_id = (Uint32) id;
  m_texture_id = (Uint32) texture_id;
}

Uint32 Animation::getID() const {
  return m_id;
}

const Math::IRect &Animation::getSpritesRange() const {
  return m_sprites_range;
}

const Math::Size &Animation::getSpritesCount() const {
  return m_sprites_count;
}

Uint32 Animation::getTextureID() const {
  return m_texture_id;
}

float32 Animation::getEachTime() const {
  return m_each_time;
}

int32 Animation::isLooping() const {
  return m_looping;
}

void FreeAnimation(Animation **anim) {
  (*anim)->~Animation();
  (*anim) = nullptr;
}
}