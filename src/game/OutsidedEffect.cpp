#include "OutsidedEffect.h"
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Game {
// OutsidedEffect class
OutsidedEffect::OutsidedEffect()
    :Component(),
    m_particle_count(0), m_max_particle_count(0), m_uv_count(0), m_radius(0.0f),
    m_generate_speed(0.0f), m_life_time(0.0f), m_scale(0.0f), m_velocity_range(0.0f),
    m_direction(0.0f),
    m_particles(nullptr), m_particle_sprites(nullptr), m_uvs(nullptr) {
  ;
}

OutsidedEffect::~OutsidedEffect() {
  ;
}

void OutsidedEffect::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("texture"));
  assert(obj.HasMember("max_particlecount"));
  assert(obj.HasMember("radius"));
  assert(obj.HasMember("generatespeed"));
  assert(obj.HasMember("lifetime"));
  assert(obj.HasMember("scale"));
  assert(obj.HasMember("velocity_range"));
  assert(obj.HasMember("uvs"));
  assert(obj.HasMember("particle"));
  assert(obj["texture"].GetString());
  assert(obj["max_particlecount"].IsUint());
  assert(obj["radius"].IsFloat());
  assert(obj["generatespeed"].IsFloat());
  assert(obj["lifetime"].IsFloat());
  assert(obj["scale"].IsFloat());
  assert(obj["velocity_range"].IsObject());
  assert(obj["uvs"].IsArray());
  assert(obj["particle"].IsObject());

  Application &app = Application::Get();
  auto &textures = app.getTextureStorage();
  const char *str_texture_id = obj["texture"].GetString();
  StringID texture_id(str_texture_id);
  Texture *tex = textures[(Uint32)texture_id];

  m_max_particle_count = obj["max_particlecount"].GetUint();
  m_radius = obj["radius"].GetFloat();
  m_generate_speed = obj["generatespeed"].GetFloat();
  m_life_time = obj["lifetime"].GetFloat();
  m_scale = obj["scale"].GetFloat();
  JsonParseMethods::ReadVector2(obj["velocity_range"].GetObject(), &m_velocity_range);
  auto load_list = obj["uvs"].GetArray();
  auto particle_json_object = obj["particle"].GetObject();

  m_uv_count = load_list.Size();

  m_particles = allocator.alloc<Particle>(m_max_particle_count);
  memset(m_particles, 0, sizeof(Particle)*m_max_particle_count);
  m_particle_sprites = allocator.alloc<Sprite*>(m_max_particle_count);
  memset(m_particle_sprites, 0, sizeof(Sprite*)*m_max_particle_count);
  m_uvs = allocator.alloc<Math::Rect>(m_uv_count);
  memset(m_uvs, 0, sizeof(Math::IRect)*m_uv_count);

  Math::Rect *now = m_uvs;
  Math::IRect read = {0, 0, 0, 0};
  for(auto iter = load_list.begin();
      iter != load_list.end();
      ++iter) {
    assert(iter->IsObject());
    JsonParseMethods::ReadIRect(iter->GetObject(), &read);
    now->x = (float32) read.x / (tex->w);
    now->w = (float32) read.w / (tex->w);
    now->y = (float32) read.y / (tex->h);
    now->h = (float32) read.h / (tex->h);
    ++now;
  }

  Uint32 uv_type = 0;
  GameObject *new_object = nullptr;
  auto *scene = Application::Get().getScene();
  for(Uint32 i=0; i<m_max_particle_count; ++i) {
    new_object = scene->createGameObject(particle_json_object);
    new_object-a>setParent(getGameObject());
    m_particle_sprites[i] = new_object->getComponent<Sprite>();
    m_particle_sprites[i]->setTexture(tex);
    m_particle_sprites[i]->setUV(m_uvs[uv_type++]);
    new_object->setAvailable(false);
    if(uv_type >= m_uv_count)
      uv_type = 0;
  }
}

void OutsidedEffect::start() {
  Component::start();
}

void OutsidedEffect::update() {
  Time &t = Time::Get();
  float32 delta_time = t.getDeltatime();
  float32 scale = 0.0f;
  for(Uint32 i=0; i<m_particle_count; ++i) {
    m_particle_sprites[i]->move(m_direction*(m_particles[i].velocity*delta_time));
    m_particle_sprites[i]->rotate(m_particles[i].velocity*delta_time);
    scale = m_scale*(m_particles[i].remain_time/m_life_time);
    m_particle_sprites[i]->setScale(scale, scale);
    m_particles[i].remain_time -= delta_time;
    if(m_particles[i].remain_time < 0.0f) {
      m_particle_sprites[i]->getGameObject()->setAvailable(false);
    }
  }

  if(isGenerating()) {
    generate();
  }
}

void OutsidedEffect::release() {
  ;
}

void OutsidedEffect::generate(const glm::vec2 &direction) {
  glm::vec2 normalized = glm::normalize(direction);
  m_direction = {normalized.x, normalized.y, 0.0f};
  m_particle_count = 0;
  setGenerating(true);
}

void OutsidedEffect::generate() {
  Time &t = Time::Get();
  srand(rand() + t.getIElapsedtime());
  auto gen = (Uint32) (m_generate_speed * t.getDeltatime());
  float32 theta = 0.0f;
  for(Uint32 i=0; i<gen; ++i) {
    if(m_particle_count >= m_max_particle_count)
      break;
    theta = Math::Constant::PIf*2*(float32)rand()/RAND_MAX;
    m_particle_sprites[m_particle_count]->setLocalPosition(cos(theta)*m_radius, sin(theta)*m_radius, 1.0f);
    m_particles[m_particle_count].velocity = m_velocity_range.x + (m_velocity_range.y-m_velocity_range.x)*(float32)(rand()/RAND_MAX);
    m_particle_sprites[m_particle_count]->setScale(m_scale, m_scale);
    m_particle_sprites[m_particle_count]->setRotation(0.0f);
    m_particles[m_particle_count].remain_time = m_life_time;
    m_particle_sprites[m_particle_count]->getGameObject()->setAvailable(true);
    ++m_particle_count;
  }
  if(m_particle_count >= m_max_particle_count) {
    setGenerating(false);
  }
}

int32 OutsidedEffect::isGenerating() {
  return m_flags.getFlag(10);
}

void OutsidedEffect::setGenerating(int32 val) {
  m_flags.setFlag(10, val);
}

}