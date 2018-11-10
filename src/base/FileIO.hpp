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

  int32 open(FILE *file, void *memory, size_t size);

  int32 write(FILE *file) const;

  int32 isAvailable() const;

  byte *getBuffer();

  const byte *getBuffer() const;

  const byte *const *getBufferPointer() const;

  size_t getSize() const;

 private:
  byte *m_buffer;
  size_t m_size;
};


// File class

FILE *OpenFile(const char *filename, const char *mode);
int64 GetFileSize(FILE *file);     // this function call rewind()
}

#endif
