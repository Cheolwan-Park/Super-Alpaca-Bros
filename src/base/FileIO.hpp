#ifndef BaseFileIO_hpp
#define BaseFileIO_hpp

#include "types.hpp"
#include "String.hpp"

namespace Base {
class FileIO {
 public:
  explicit FileIO();

  explicit FileIO(FILE *file, void *memory, size_t size);

  ~FileIO();

  void open(void *memory, size_t size);

  int32_t open(FILE *file, void *memory, size_t size);

  int32_t write(FILE *file) const;

  int32_t isAvailable() const;

  char *getBuffer();

  const char *getBuffer() const;

  const char *const *getBufferPointer() const;

  size_t getSize() const;

 private:
  char *m_buffer;
  size_t m_size;
};


// File class

FILE *OpenFile(const char *filename, const char *mode);
int64_t GetFileSize(FILE *file);     // this function call rewind()
}

#endif
