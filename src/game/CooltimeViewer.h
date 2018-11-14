#ifndef ALPACAS_COOLTIMEVIEWER_H
#define ALPACAS_COOLTIMEVIEWER_H

#include "base.hpp"
#include "AlpacaAction.hpp"

using namespace Base;


namespace Game {
class CooltimeViewer : public Component {
 public:
  COMPONENT(CooltimeViewer);

  CooltimeViewer();

  CooltimeViewer(const CooltimeViewer &other) = delete;

  ~CooltimeViewer() override;

  CooltimeViewer &operator=(const CooltimeViewer &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

 private:
  Uint32 m_alpaca_index;
  Uint32 m_action_index;
  Sprite *m_sprite;
  Math::Rect m_default_uv;
  glm::vec3 m_default_position;
  glm::vec2 m_default_scale;
  Alpaca::Action *m_action;
  Alpaca::Alpaca *m_alpaca;
};
}

#endif //ALPACAS_COOLTIMEVIEWER_H
