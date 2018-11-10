#ifndef BaseCamera_hpp
#define BaseCamera_hpp

#include "types.hpp"
#include "Json.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif
#include <glm/glm.hpp>

namespace Base {
class Camera {
 public:
  Camera();

  Camera(const Camera &other);

  virtual ~Camera();

  Camera &operator=(const Camera &other);

  void initWithJson(const rapidjson::Value::Object &obj);

  void setProjection(float32 left, float32 right,
                     float32 bottom, float32 top,
                     float32 n, float32 f);

  Uint32 getID() const;

  const glm::mat4 &getViewMatrix() const;

  const glm::mat4 &getProjectionMatrix() const;

  const glm::vec3 &getLeftTopNear() const;

  const glm::vec3 &getRightBottomFar() const;

 private:
  Uint32 m_id;
  glm::mat4 m_view;
  glm::mat4 m_projection;
  glm::vec3 m_left_top_near;
  glm::vec3 m_right_bottom_far;
};
}

#endif
