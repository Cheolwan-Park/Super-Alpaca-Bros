#include "ShaderProgram.hpp"

namespace Base {
// ShaderProgram class

ShaderProgram::ShaderProgram()
    : m_id(0), m_program(0), m_vertex(0), m_fragment(0), m_uniform_locactions() {
  ;
}

ShaderProgram::~ShaderProgram() {
  release();
}

int32 ShaderProgram::init(const FileIO &vert, const FileIO &frag) {
  assert(0 == m_program);
  assert(vert.isAvailable());
  assert(frag.isAvailable());

  int32 status_vertex = GL_TRUE, status_fragment = GL_TRUE, status_link = GL_TRUE;
  int32 log_len = 0;

  // compile vertex shader
  m_vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(m_vertex, 1, vert.getBufferPointer(), 0);
  glCompileShader(m_vertex);

  glGetShaderiv(m_vertex, GL_COMPILE_STATUS, &status_vertex);
  if (GL_FALSE == status_vertex) {
    glGetShaderiv(m_vertex, GL_INFO_LOG_LENGTH, &log_len);
    auto *info = (GLchar *) malloc(log_len);
    glGetShaderInfoLog(m_vertex, log_len, &log_len, info);
    fprintf(stderr, "compiling vertex shader failed, info:\n%s", info);
    free(info);
    return RET_FAILED;
  }

  // compile fragment shader
  m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(m_fragment, 1, frag.getBufferPointer(), 0);
  glCompileShader(m_fragment);

  glGetShaderiv(m_fragment, GL_COMPILE_STATUS, &status_fragment);
  if (GL_FALSE == status_fragment) {
    glGetShaderiv(m_fragment, GL_INFO_LOG_LENGTH, &log_len);
    auto *info = (GLchar *) malloc(log_len);
    glGetShaderInfoLog(m_fragment, log_len, &log_len, info);
    fprintf(stderr, "compiling fragment shader failed, info:\n%s", info);
    free(info);
    glDeleteShader(m_vertex);
    m_vertex = m_fragment = 0;
    return RET_FAILED;
  }

  // link shader
  m_program = glCreateProgram();
  glAttachShader(m_program, m_vertex);
  glAttachShader(m_program, m_fragment);
  glLinkProgram(m_program);

  glGetProgramiv(m_program, GL_LINK_STATUS, &status_link);
  if (GL_FALSE == status_link) {
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_len);
    auto *info = (GLchar *) malloc(log_len);
    glGetProgramInfoLog(m_program, log_len, &log_len, info);
    fprintf(stderr, "link shaders failed, info:\n%s", info);
    free(info);
    glDeleteShader(m_vertex);
    glDeleteShader(m_fragment);
    m_program = m_vertex = m_fragment = 0;
    return RET_FAILED;
  }

  m_uniform_locactions[0] = glGetUniformLocation(m_program, "vp");
  m_uniform_locactions[1] = glGetUniformLocation(m_program, "tex");

  return RET_SUCC;
}

int32 ShaderProgram::initWithJson(const rapidjson::Value::Object &obj, void *vertex_mem, void *fragment_mem) {
  assert(obj.HasMember("name"));
  assert(obj.HasMember("vert"));
  assert(obj.HasMember("frag"));
  assert(obj["name"].IsString());
  assert(obj["vert"].IsString());
  assert(obj["frag"].IsString());

  const char *shader_name = obj["name"].GetString();
  StringID id(shader_name);
  m_id = (Uint32) id;

  // open shader files
  const char *vert_filename = obj["vert"].GetString();
  const char *frag_filename = obj["frag"].GetString();
  FILE *vert = nullptr, *frag = nullptr;
  int64 size_vert = 0, size_frag = 0;
  FileIO vert_io, frag_io;

  String128 path(Directories::Shader);
  path += vert_filename;
  vert = OpenFile(path.c_str(), "rb");

  path.clear();
  path = Directories::Shader;
  path += frag_filename;
  frag = OpenFile(path.c_str(), "rb");

  size_vert = GetFileSize(vert);
  size_frag = GetFileSize(frag);

  if (nullptr == vertex_mem && nullptr == fragment_mem) {
    vertex_mem = malloc(size_vert + 1);
    fragment_mem = malloc(size_frag + 1);
  } else {
    vertex_mem = realloc(vertex_mem, size_vert + 1);
    fragment_mem = realloc(fragment_mem, size_frag + 1);
  }
  memset(vertex_mem, 0, size_vert + 1);
  memset(fragment_mem, 0, size_frag + 1);

  if (RET_SUCC != vert_io.open(vert, vertex_mem, size_vert)) {
    return RET_FAILED;
  }

  if (RET_SUCC != frag_io.open(frag, fragment_mem, size_frag)) {
    return RET_FAILED;
  }

  if (RET_SUCC != init(vert_io, frag_io)) {
    return RET_FAILED;
  }

  return RET_SUCC;
}

void ShaderProgram::release() {
  if (0 != m_program) {
    glDeleteShader(m_vertex);
    glDeleteShader(m_fragment);
    glDeleteProgram(m_program);
    m_vertex = m_fragment = m_program = 0;
  }
}

Uint32 ShaderProgram::getID() const {
  return m_id;
}

GLuint ShaderProgram::getProgram() const {
  return m_program;
}

GLint ShaderProgram::getViewProjectionLocation() const {
  return m_uniform_locactions[0];
}

GLint ShaderProgram::getTextureLocation() const {
  return m_uniform_locactions[1];
}

void FreeShader(ShaderProgram **prog) {
  (*prog)->~ShaderProgram();
  (*prog) = nullptr;
}
}










