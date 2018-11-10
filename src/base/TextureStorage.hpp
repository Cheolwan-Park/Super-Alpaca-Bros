#ifndef BaseTextureStorage_hpp
#define BaseTextureStorage_hpp

#include "types.hpp"
#include "SDL.hpp"
#include "Json.hpp"

namespace Base {
struct Texture {
  GLuint hash_id;
  GLuint id;
  GLint w;
  GLint h;
};

void LoadTexture(Texture *texture, const rapidjson::Value::Object &obj);
GLuint LoadTexture(const char *filename, GLint *w, GLint *h);

void FreeTexture(Texture **tex);
}

#endif
