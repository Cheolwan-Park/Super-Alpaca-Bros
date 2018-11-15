#include "AlpacaWalkSound.h"
#include "GameManager.hpp"

namespace Game {
namespace Alpaca {
WalkSound::WalkSound()
    : Component(), m_walk_chunk_id(0), m_walk_chunk_channel(-1), m_alpacas() {
  ;
}

WalkSound::~WalkSound() {
  ;
}

void WalkSound::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  Component::initWithJson(obj, allocator);

  assert(obj.HasMember("chunk"));
  assert(obj["chunk"].IsString());

  const char *str_walk_chunk = obj["chunk"].GetString();
  StringID walk_chunk_id(str_walk_chunk);
  m_walk_chunk_id = (Uint32)walk_chunk_id;
}

void WalkSound::start() {
  auto *game_manager = GameManager::GetGlobal();
  assert(game_manager);
  m_alpacas[0] = game_manager->getAlpaca(0);
  m_alpacas[1] = game_manager->getAlpaca(1);
  assert(m_alpacas[0]);
  assert(m_alpacas[1]);

  Component::start();
}

void WalkSound::update() {
  Component::update();

  int32 play = false;
  for(Uint32 i=0; i<2; ++i) {
    if(m_alpacas[i]->isGrounded() && (m_alpacas[i]->isMoving())) {
      play = true;
    }
  }

  auto &mixer = SDL::Mixer::Get();
  if(play) {
    if(-1 == m_walk_chunk_channel) {
      auto &chunks = Application::Get().getChunkStorage();
      m_walk_chunk_channel = mixer.playChunk(chunks[m_walk_chunk_id], -1);
    } else {
      mixer.resume(m_walk_chunk_channel);
    }
  } else {
    mixer.pause(m_walk_chunk_channel);
  }
}

void WalkSound::release() {
  if (-1 != m_walk_chunk_id) {
    auto &mixer = SDL::Mixer::Get();
    mixer.halt(m_walk_chunk_id);
  }
}

}
}
