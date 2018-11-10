#ifndef GameCameraController_hpp
#define GameCameraController_hpp

#include "base.hpp"

using namespace Base;

namespace Game {
class GameManager;

class CameraController : public Component {
 public:
  COMPONENT(CameraController);

  CameraController();

  CameraController(const CameraController &other) = delete;

  ~CameraController() override;

  CameraController &operator=(const CameraController &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  int32 isSmooth() const;

  void setSmooth(int32 val);

 private:
  GameManager *m_game_manager;
  float32 m_margin;
  float32 m_camera_speed;
  float32 m_expand_speed;
  float32 m_expanded;
  glm::vec2 m_view_ratio;
  float32 m_min_camera_size;
  float32 m_max_camera_size;

  /*
   * flags
   * Component's flag
   * 2 : isSmooth
   */
};
}

#endif