#include "SDLMixer.h"
#include "types.hpp"
#include "String.hpp"
#include <assert.h>

namespace SDL {
Mixer::Mixer()
    :m_music(nullptr) {

}

Mixer::~Mixer() {
  if(m_music)
    Mix_FreeMusic(m_music);
}

void Mixer::changeMusic(const char *filename) {
  if(m_music)
    Mix_FreeMusic(m_music);
  Base::String128 path(Base::Directories::Sound);
  path += filename;
  m_music = Mix_LoadMUS(path.c_str());
  if(!m_music) {
    fprintf(stderr, "Failed Mix_LoadMUS() : %s\n", Mix_GetError());
  }
  assert(m_music);
  Mix_PlayMusic(m_music, -1);
}

void Mixer::setMusicVolume(Uint32 volume) {
  assert(volume < MIX_MAX_VOLUME);
  Mix_VolumeMusic(volume);
}

void Mixer::pauseMusic() {
  Mix_PauseMusic();
}

void Mixer::resumeMusic() {
  Mix_ResumeMusic();
}

void Mixer::resume(int32_t channel) {
  Mix_Resume(channel);
}

void Mixer::pause(int32_t channel) {
  Mix_Pause(channel);
}

void Mixer::halt(int32_t channel) {
  Mix_HaltChannel(channel);
}

int32_t Mixer::isPaused(int32_t channel) {
  return Mix_Paused(channel);
}

int32_t Mixer::playChunk(Mix_Chunk *chunk) {
  return Mix_PlayChannel(-1, chunk, 0);
}

int32_t Mixer::playChunk(Mix_Chunk *chunk, int32_t loops) {
  return Mix_PlayChannel(-1, chunk, loops);
}

int32_t Mixer::playChunk(int32_t channel, Mix_Chunk *chunk, int32_t loops) {
  return Mix_PlayChannel(channel, chunk, loops);
}

void Mixer::setVolume(Mix_Chunk *chunk, Uint32 volume) {
  assert(volume < MIX_MAX_VOLUME);
  Mix_VolumeChunk(chunk, volume);
}

Mix_Chunk *Mixer::loadChunk(const char *filename) {
  Mix_Chunk *result = nullptr;
  Base::String128 path(Base::Directories::Sound);
  path += filename;
  result = Mix_LoadWAV(path.c_str());
  if(!result) {
    fprintf(stderr, "Failed Mix_LoadWAV() : %s\n", Mix_GetError());
  }
  assert(result);
  return result;
}

Mixer &Mixer::Get() {
  static Mixer instance;
  return instance;
}

void FreeChunk(Mix_Chunk **chunk) {
  if(*chunk) {
    Mix_FreeChunk(*chunk);
  }
  (*chunk) = nullptr;
}

}