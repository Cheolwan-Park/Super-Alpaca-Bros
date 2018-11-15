//
// Created by Cheolwan Park on 15/11/2018.
//

#ifndef ALPACAS_SDLMIXER_H
#define ALPACAS_SDLMIXER_H

#include "SDL.hpp"

namespace SDL {
class Mixer {
 private:
  Mixer();

 public:
  Mixer(const Mixer &other) = delete;

  ~Mixer();

  Mixer &operator=(const Mixer &other) =delete;

  void changeMusic(const char *filename);

  void setMusicVolume(Uint32 volume);

  void pauseMusic();

  void resumeMusic();

  void resume(int32_t channel);

  void pause(int32_t channel);

  void halt(int32_t channel);

  int32_t isPaused(int32_t channel);

  int32_t playChunk(Mix_Chunk *chunk);

  int32_t playChunk(Mix_Chunk *chunk, int32_t loops);

  int32_t playChunk(int32_t channel, Mix_Chunk *chunk, int32_t loops);

  void setVolume(Mix_Chunk *chunk, Uint32 volume);

  Mix_Chunk *loadChunk(const char *filename);

 public:
  static Mixer &Get();

 private:
  Mix_Music *m_music;

};

void FreeChunk(Mix_Chunk **chunk);

}

#endif //ALPACAS_SDLMIXER_H
