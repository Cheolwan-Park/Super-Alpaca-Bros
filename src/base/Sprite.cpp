#include "Sprite.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Time.hpp"
#include "Application.hpp"
#include "Scene.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Base {
// Drawable class
Component *Drawable::Factory(const rapidjson::Value::Object &obj, StackAllocator &allocator, GameObject *gameobject) {
  fprintf(stderr, "Drawable is abstract class\n");
  return nullptr;
}

Drawable::Drawable()
    : Component() {
  ;
}

Drawable::Drawable(const Drawable &other)
    : Component(other) {
  ;
}

Drawable::~Drawable() {
  ;
}

Drawable &Drawable::operator=(const Drawable &other) {
  assert(this != &other);
  Component::operator=(other);
  return (*this);
}

void Drawable::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("storage"));
  assert(obj["storage"].IsString());

  const char *storage_name = obj["storage"].GetString();
  Uint32 storage_id = CompileTimeHash::runtime_hash(storage_name, strlen(storage_name));

  if ("none"_hash != storage_id) {
    Scene *scene = Application::Get().getScene();
    assert(scene);
    DrawableStorage *storage = scene->getDrawableStorage(storage_id);
    assert(storage);
    storage->add(this);
  }
}

void Drawable::start() {
  Component::start();
}

void Drawable::update() {
  Component::update();
}

void Drawable::release() {
  ;
}

void Drawable::setDrawer(DrawableStorage *drawer) {
  drawer->add(this);
}

// Sprite class
const glm::vec3 Sprite::vertexes[4] =
    {
        {-0.5f, 0.5f, 1.0f},   // top left
        {0.5f, 0.5f, 1.0f},   // top right
        {-0.5f, -0.5f, 1.0f},   // bottom left
        {0.5f, -0.5f, 1.0f}    // bottom right
    };

const glm::vec2 Sprite::uvs[4] =
    {
        {0.0f, 1.0f},   // top left
        {1.0f, 1.0f},   // top right
        {0.0f, 0.0f},   // bottom left
        {1.0f, 0.0f}    // bottom right
    };

Sprite::Sprite()
    : Drawable(), m_vao(), m_uv(), m_tex(nullptr) {
  ;
}

Sprite::Sprite(const Sprite &other)
    : Drawable(other), m_vao(), m_uv(other.m_uv), m_tex(other.m_tex) {
  ;
}

Sprite::~Sprite() {
  ;
}

Sprite &Sprite::operator=(const Sprite &other) {
  assert(this != &other);
  Drawable::operator=(other);
  this->m_uv = other.m_uv;
  this->m_tex = other.m_tex;
  return (*this);
}

void Sprite::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Drawable::initWithJson(obj, allocator);

  assert(obj.HasMember("uv"));
  assert(obj.HasMember("texture"));
  assert(obj["uv"].IsObject());
  assert(obj["texture"].IsString());

  Application &app = Application::Get();
  auto &textures = app.getTextureStorage();
  const char *str_texture_id = obj["texture"].GetString();
  StringID texture_id(str_texture_id);
  m_tex = textures[(Uint32)texture_id];

  Math::IRect rect({0, 0, 0, 0});
  JsonParseMethods::ReadIRect(obj["uv"].GetObject(), &rect);
  setUV(rect);
}

void Sprite::start() {
  initVAO();
  glBindVertexArray(m_vao.id);
  updateVBO();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  Drawable::start();
}

void Sprite::update() {
  Drawable::update();
}

void Sprite::release() {
  releaseVAO();
  Drawable::release();
}

void Sprite::draw() {
  if (m_tex && isAvailable()) {
    glBindVertexArray(m_vao.id);

    if (!getGameObject()->isStatic())
      updateVBO();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex->id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
  }
}

void Sprite::updateVBO() {
  constexpr GLbitfield map_buffer_flag = GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;

  if (needUpdateUV()) {
    glBindBuffer(GL_ARRAY_BUFFER, m_vao.vbo[1]);
    auto *uv_buffer = (glm::vec2 *) glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(uvs), map_buffer_flag);
    float32 right = m_uv.x + m_uv.w;
    float32 top = m_uv.y + m_uv.h;
    uv_buffer[0] = {m_uv.x, top};
    uv_buffer[1] = {right, top};
    uv_buffer[2] = {m_uv.x, m_uv.y};
    uv_buffer[3] = {right, m_uv.y};
    glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, sizeof(uvs));
    glUnmapBuffer(GL_ARRAY_BUFFER);
    setNeedUpdateUV(false);
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_vao.vbo[0]);
  glm::vec3 *vert_buffer = (glm::vec3 *) glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(vertexes), map_buffer_flag);
  glm::mat3x3 model(1.0f);
  getGameObject()->getModel(&model);
  for (Uint32 i = 0; i < 4; ++i) {
    vert_buffer[i] = model * vertexes[i];
  }
  glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, sizeof(vertexes));
  glUnmapBuffer(GL_ARRAY_BUFFER);
}

void Sprite::initVAO() {
  if (0 != m_vao.id)
    return;
  glGenVertexArrays(1, &m_vao.id);
  glBindVertexArray(m_vao.id);
  glGenBuffers(2, m_vao.vbo);

  // check static
  GLenum usage = GL_DYNAMIC_DRAW;
  if (getGameObject()->isStatic())
    usage = GL_STATIC_DRAW;


  // vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_vao.vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, usage);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // texcoord buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_vao.vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, usage);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Sprite::releaseVAO() {
  if (0 == m_vao.id)
    return;
  glDeleteBuffers(2, m_vao.vbo);
  glDeleteVertexArrays(1, &m_vao.id);
}

const Math::Rect &Sprite::getUV() const {
  return m_uv;
}

const Texture *Sprite::getTexture() const {
  return m_tex;
}

void Sprite::setUV(const Math::Rect &rect) {
  m_uv = rect;
  setNeedUpdateUV(true);
}

void Sprite::setUV(const Math::IRect &rect) {
  m_uv.x = (float32) rect.x / (m_tex->w);
  m_uv.w = (float32) rect.w / (m_tex->w);
  m_uv.y = (float32) rect.y / (m_tex->h);
  m_uv.h = (float32) rect.h / (m_tex->h);
  setNeedUpdateUV(true);
}

void Sprite::setTexture(const Texture *val) {
  m_tex = val;
}

int32 Sprite::needUpdateUV() const {
  return m_flags.getFlag(9);
}

void Sprite::setNeedUpdateUV(int32 val) {
  m_flags.setFlag(9, val);
}


// SpriteDrawer class

void DefaultRenderSettingFun() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

DrawableStorage::DrawableStorage()
    : m_id(0), m_order(0), m_shader(nullptr), m_render_setting(DefaultRenderSettingFun),
      m_len(0), m_drawables(nullptr) {
  ;
}

DrawableStorage::~DrawableStorage() {
  
}

void DrawableStorage::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  assert(obj.HasMember("name"));
  assert(obj.HasMember("shader"));
  assert(obj.HasMember("camera"));
  assert(obj.HasMember("order"));
  assert(obj.HasMember("size"));
  assert(obj["name"].IsString());
  assert(obj["shader"].IsString());
  assert(obj["camera"].IsString());
  assert(obj["order"].IsUint());
  assert(obj["size"].IsUint());

  const char *storage_name = obj["name"].GetString();
  const char *shader_name = obj["shader"].GetString();
  const char *camera_name = obj["camera"].GetString();
  StringID storage_id(storage_name), shader_id(shader_name), camera_id(camera_name);
  m_id = (Uint32) storage_id;
  m_order = obj["order"].GetUint();

  m_len = obj["size"].GetUint();
  m_drawables = allocator.alloc<Type>(m_len);
  memset(m_drawables, 0, sizeof(Type) * m_len);

  auto &app = Application::Get();
  auto &shaders = app.getShaderStorage();
  auto *scene = app.getScene();
  assert(scene);
  setShader(shaders[(Uint32) shader_id]);

  setCamera(scene->getCamera((Uint32) camera_id));
}

void DrawableStorage::setCamera(Camera *camera) {
  m_camera = camera;
}

void DrawableStorage::setShader(ShaderProgram *shader) {
  m_shader = shader;
}

void DrawableStorage::setRenderSettingFun(const std::function<void(void)> &func) {
  m_render_setting = func;
}

Drawable *DrawableStorage::add(Drawable *drawable) {
  for (Uint32 i = 0; i < m_len; ++i) {
    if (!m_drawables[i]) {
      m_drawables[i] = drawable;
      return drawable;
    }
  }
  fprintf(stderr, "there is no space in DrawableStorage\n");
  exit(-1);
}

void DrawableStorage::drawDrawables() {
  if (m_shader && m_camera) {
    glUseProgram(m_shader->getProgram());
    m_render_setting();

    // set vp uniform
    const glm::mat4 &proj = m_camera->getProjectionMatrix();
    const glm::mat4 &view = m_camera->getViewMatrix();
    glm::mat4 vp = proj * view;
    glUniformMatrix4fv(m_shader->getViewProjectionLocation(),
                       1, GL_FALSE,
                       glm::value_ptr(vp));
    glUniform1i(m_shader->getTextureLocation(), 0);

    for (Uint32 i = 0; i < m_len; ++i) {
      if (m_drawables[i]
          && m_drawables[i]->isAvailable()
          && m_drawables[i]->getGameObject()->isAvailable()) {
        m_drawables[i]->draw();
      }
    }
  }
}

void DrawableStorage::checkDeleted() {
  for (Uint32 i = 0; i < m_len; ++i) {
    if (m_drawables[i] && m_drawables[i]->getGameObject()->isDeleted()) {
      m_drawables[i] = nullptr;
    }
  }
}

Uint32 DrawableStorage::getID() const {
  return m_id;
}

Uint32 DrawableStorage::getOrder() const {
  return m_order;
}
}










