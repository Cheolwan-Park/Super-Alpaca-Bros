#ifndef ALPACAS_BANNER_H
#define ALPACAS_BANNER_H

#include "base.hpp"

using namespace Base;

namespace Game {
class BannerMover : public Component {
 public:
  COMPONENT(BannerMover);

  BannerMover();

  BannerMover(const BannerMover &other) = delete;

  ~BannerMover() override;

  BannerMover &operator=(const BannerMover &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void show();

  void hide();

  float32 getDuration() const;

  int32 isMoving() const;

  int32 isShowed() const;

 private:
  void setMoving(int32 val);

  void setShowed(int32 val);

 private:
  float32 m_elapsed_time;
  float32 m_duration;
  glm::vec3 m_hide_position;
  glm::vec3 m_show_position;
  glm::vec3 m_target_position;
  glm::vec3 m_start_position;

  /*
   * flags
   * Component's flags
   * 2 : isMoving
   * 3 : isShowed
   */
};

class WinnerBanner : public Component {
 public:
  COMPONENT(WinnerBanner);

  WinnerBanner();

  WinnerBanner(const WinnerBanner &other) = delete;

  ~WinnerBanner() override;

  WinnerBanner &operator=(const WinnerBanner &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void show(Uint32 winner);

  void hide();

 private:
  static WinnerBanner *global;
 public:
  static WinnerBanner *GetGlobal();

 private:
  float32 m_elapsed_time;
  float32 m_wait_time;
  float32 m_duration;
  float32 m_rotate;
  float32 m_max_scale;
  float32 m_default_rotation;
  glm::vec2 m_default_scale;
  float32 m_rotation;
  glm::vec2 m_scale;
  Math::IRect m_win_banner_uvs[2];
  Sprite *m_sprite;
  BannerMover *m_mover;
};

}

#endif //ALPACAS_BANNER_H
