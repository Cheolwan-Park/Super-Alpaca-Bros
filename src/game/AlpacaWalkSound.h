#ifndef ALPACAS_ALPACAWALKSOUND_H
#define ALPACAS_ALPACAWALKSOUND_H

#include "base.hpp"
#include "Alpaca.hpp"

using namespace Base;

namespace Game {
namespace Alpaca {
class WalkSound : public Component {
 public:
  COMPONENT(WalkSound);

  WalkSound();

  WalkSound(const WalkSound &other) = delete;

  ~WalkSound() override;

  WalkSound &operator=(const WalkSound &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void start() override;

  void update() override;

  void release() override;

 private:
  Uint32 m_walk_chunk_id;
  int32 m_walk_chunk_channel;
  Alpaca *m_alpacas[2];
};
}
}

#endif //ALPACAS_ALAPAWALKSOUND_H
