#ifndef BaseShaderProgram_hpp
#define BaseShaderProgram_hpp

#include "types.hpp"
#include "FileIO.hpp"
#include "Json.hpp"
#define GL_SILENCE_DEPRECATION
#ifdef _WIN32
#include <GL/glew.h>
#include <GL/gl.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glext.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

namespace Base {
using namespace Base;

class ShaderProgram {
 public:
  ShaderProgram();

  ShaderProgram(const ShaderProgram &other) = delete;

  ~ShaderProgram();

  ShaderProgram &operator=(const ShaderProgram &other) = delete;

  int32_t init(const FileIO &vert, const FileIO &frag);

  int32_t initWithJson(const rapidjson::Value::Object &obj);

  void release();

  uint32_t getID() const;

  GLuint getProgram() const;

  GLint getViewProjectionLocation() const;

  GLint getTextureLocation() const;

 private:
  uint32_t m_id;
  GLuint m_program;
  GLuint m_vertex;
  GLuint m_fragment;
  GLint m_uniform_locactions[2]; // 0 : vp, 1 : tex
};

void FreeShader(ShaderProgram **prog);
}

#endif





