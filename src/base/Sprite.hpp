#ifndef BaseSprite_hpp
#define BaseSprite_hpp

#include "TextureStorage.hpp"
#include "FileIO.hpp"
#include "Math.hpp"
#include "Component.hpp"
#include "Storage.hpp"
#include <functional>

namespace Base {
class DrawableStorage;
class ShaderProgram;
class Camera;

class Drawable : public Component {
 public:
  static Component *Factory(const rapidjson::Value::Object &obj, StackAllocator &allocator, GameObject *gameobject);

 public:
  COMPONENT(Drawable);

  Drawable();

  // not copying id
  Drawable(const Drawable &other);

  ~Drawable() override;

  // not copying id
  Drawable &operator=(const Drawable &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  virtual void draw() = 0;

  virtual void updateVBO() = 0;

  void setDrawer(DrawableStorage *drawer);
};

class Sprite : public Drawable {
 private:
  static const glm::vec3 vertexes[4];
  static const glm::vec2 uvs[4];

 public:
  COMPONENT(Sprite);

  Sprite();

  Sprite(const Sprite &other);

  ~Sprite() override;

  Sprite &operator=(const Sprite &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void draw() override;

  void updateVBO() override;

  // get
  const Math::Rect &getUV() const;

  const Texture *getTexture() const;

  // set
  void setUV(const Math::Rect &rect);

  void setUV(const Math::IRect &rect);

  void setTexture(const Texture *val);

 private:
  struct VAO {
    GLuint id;
    GLuint vbo[2];
  };
  void initVAO();

  void releaseVAO();

  int32_t needUpdateUV() const;

  void setNeedUpdateUV(int32_t val);

 private:
  VAO m_vao;
  Math::Rect m_uv;
  const Texture *m_tex;
  /* flags
   * Drawable's flags
   * 9 : update uv
   */
};

class DrawableStorage {
 public:
  typedef Drawable *Type;

  DrawableStorage();

  DrawableStorage(const DrawableStorage &other) = delete;

  ~DrawableStorage();

  DrawableStorage &operator=(const DrawableStorage &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

  void setCamera(Camera *camera);

  void setShader(ShaderProgram *shader);

  void setRenderSettingFun(const ::std::function<void(void)> &func);

  Drawable *add(Drawable *drawable);

  void drawDrawables();

  void checkDeleted();

  // get
  uint32_t getID() const;

  uint32_t getOrder() const;

 private:
  uint32_t m_id;
  uint32_t m_order;
  Camera *m_camera;
  ShaderProgram *m_shader;
  std::function<void(void)> m_render_setting;
  uint32_t m_len;
  Type *m_drawables;
};
}

#endif
