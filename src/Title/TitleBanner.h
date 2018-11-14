#ifndef ALPACAS_TITLEBANNER_H
#define ALPACAS_TITLEBANNER_H

#include "base.hpp"

using namespace Base;
namespace Title {
class TitleBanner : public Component {
 public:
  COMPONENT(TitleBanner);

  TitleBanner();

  TitleBanner(const TitleBanner &other) = delete;

  ~TitleBanner() override;

  TitleBanner &operator=(const TitleBanner &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

 private:
  float32 m_elapsed_time;
  float32 m_wait_time;
  float32 m_duration;
  float32 m_rotate;
  float32 m_scale;
  float32 m_default_rotation;
  glm::vec2 m_default_scale;
};

class Rotator : public Component {
 public:
  COMPONENT(Rotator);

  Rotator();

  Rotator(const Rotator &other) = delete;

  ~Rotator() override;

  Rotator &operator=(const Rotator &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  float32 getSpeed() const;

  void setSpeed(float32 v);

 private:
  float32 m_speed;
};

class ScaleRepeater : public Component {
 public:
  COMPONENT(ScaleRepeater);

  ScaleRepeater();

  ScaleRepeater(const ScaleRepeater &other) = delete;

  ~ScaleRepeater() override;

  ScaleRepeater &operator=(const ScaleRepeater &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  float32 getDuration() const;

  float32 getMaxScale() const;

  void setDuration(float32 duration);

  void setMaxScale(float32 scale);

 private:
  float32 m_elapsed_time;
  float32 m_duration;
  float32 m_scale;
  glm::vec2 m_default_scale;
};

class ZoomTransition : public Component {
 public:
  COMPONENT(ZoomTransition);

  ZoomTransition();

  ZoomTransition(const ZoomTransition &other) = delete;

  ~ZoomTransition() override;

  ZoomTransition &operator=(const ZoomTransition &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void transition();

  int32 isTransitioning() const;

 private:
  String128 m_load_scene;
  float32 m_duration;
  float32 m_elapsed_time;
  glm::vec3 m_target_ltn;
  glm::vec3 m_target_rbf;
  glm::vec3 m_default_ltn;
  glm::vec3 m_default_rbf;
  Camera *m_camera;

  /*
   * flags
   * Component's flags
   * 2 : isTransitioning
   */
};
}
#endif //ALPACAS_TITLEBANNER_H
