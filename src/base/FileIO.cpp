#include "FileIO.hpp"
#include <stdio.h>

namespace Base {
FileIO::FileIO()
    : m_buffer(nullptr), m_size(0) {
  ;
}

FileIO::FileIO(FILE *file, void *memory, size_t size)
    : m_buffer(nullptr), m_size(0) {
  open(file, memory, size);
}

FileIO::~FileIO() {
  // memory will be freed by other object
}

void FileIO::open(void *memory, size_t size) {
  assert(nullptr != memory);
  assert(0 != size);
  m_buffer = (char*) memory;
  m_size = size;
}

int32_t FileIO::open(FILE *file, void *memory, size_t size) {
  assert(file != nullptr);

  open(memory, size);

  if (m_size != fread(m_buffer, 1, m_size, file))
    return RET_FAILED;

  return RET_SUCC;
}

int32_t FileIO::write(FILE *file) const {
  assert(nullptr != file);
  assert(isAvailable());

  if (m_size != fwrite((void *) m_buffer, sizeof(char), m_size, file))
    return RET_FAILED;

  return RET_SUCC;
}

int32_t FileIO::isAvailable() const {
  return (nullptr != m_buffer && 0 != m_size);
}

char *FileIO::getBuffer() {
  return m_buffer;
}

const char *FileIO::getBuffer() const {
  return m_buffer;
}

const char *const *FileIO::getBufferPointer() const {
  return &m_buffer;
}

size_t FileIO::getSize() const {
  return m_size;
}

// relative functions
FILE *OpenFile(const char *filename, const char *mode) {
  FILE *result = nullptr;
  result = fopen(filename, mode);
  if (!result)
    printf("failed open %s\n", filename);
  assert(result);
  return result;
}

int64_t GetFileSize(FILE *file) {
  int64_t result = 0;
  fseek(file, 0, SEEK_END);
  result = ftell(file);
  rewind(file);
  return result;
}
}















