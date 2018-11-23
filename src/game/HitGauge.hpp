#ifndef GameHitGauge_hpp
#define GameHitGauge_hpp

#include "base.hpp"

using namespace Base;

namespace Game {
class HitGauge : public Component {
 public:
  COMPONENT(HitGauge);

  HitGauge();

  HitGauge(const HitGauge &other);

  ~HitGauge() override;

  HitGauge &operator=(const HitGauge &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void resetGauge();

  void gaugeUp(float32 val);

  float32 getGauge() const;

  float32 GetFactor() const;

 private:
  float32 m_gauge;
  float32 m_max_gauge;
  float32 m_min_factor;
  float32 m_max_factor;

  // animation
  float32 m_animation_time;
  float32 m_remain_time;
  float32 m_start;
  float32 m_target;
};

class NumberViewer : public Sprite {
 public:
  COMPONENT(NumberViewer);

  NumberViewer();

  NumberViewer(const NumberViewer &other);

  ~NumberViewer() override;

  NumberViewer &operator=(const NumberViewer &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

  void setNumber(uint32_t n);

  void setNumberSheet(Animation *sheet);

 private:
  Animation *m_number_sheet;
};

class HitGaugeViewer : public Component {
 public:
  COMPONENT(HitGaugeViewer);

  HitGaugeViewer();

  HitGaugeViewer(const HitGaugeViewer &other) = delete;

  ~HitGaugeViewer() override;

  HitGaugeViewer &operator=(const HitGaugeViewer &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

 private:
  uint32_t m_alpaca_index;
  HitGauge *m_alpaca_hit_gauge;
  FixedArray<NumberViewer*, 3> m_number_viewers;
};
}

#endif