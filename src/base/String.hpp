#ifndef BaseString_hpp
#define BaseString_hpp

#include "types.hpp"
#include "Allocator.hpp"
#include <memory.h>
#include <string.h>
#include <stdarg.h>

namespace Base {
template<size_t MaxSize>
class String {
 public:
  String()
      : m_str(), m_size(0) {
    memset(m_str, 0, MaxSize);
  }

  explicit String(const char *str)
      : m_str(), m_size(strlen(str)) {
    memset(m_str, 0, MaxSize);
    for (size_t i = 0; i < m_size; ++i) {
      m_str[i] = str[i];
    }
  }

  explicit String(const char *format, va_list ap)
      : m_str(), m_size(0) {
    memset(m_str, 0, MaxSize);
    m_size = vsnprintf(m_str, MaxSize, format, ap);
  }

  String(const String &other)
      : m_str(), m_size(other.m_size) {
    memset(m_str, 0, MaxSize);
    for (size_t i = 0; i < m_size; ++i) {
      this->m_str[i] = other.m_str[i];
    }
  }

  // member functions
  void clear() {
    memset(m_str, 0, MaxSize);
    m_size = 0;
  }

  void sprint(const char *format, ...) {
    memset(m_str, 0, MaxSize);
    va_list ap;
    va_start(ap, format);
    m_size = vsnprintf(m_str, MaxSize, format, ap);
    va_end(ap);
  }

  void sprint(const char *format, va_list ap) {
    memset(m_str, 0, MaxSize);
    m_size = vsnprintf(m_str, MaxSize, format, ap);
  }

  String &append(const String &other) {
    size_t size_sum = this->m_size + other.m_size;
    assert(size_sum <= MaxSize);
    size_t other_i = 0;
    for (size_t i = m_size; i < size_sum; ++i) {
      this->m_str[i] = other.m_str[other_i];
      ++other_i;
    }
    this->m_size = size_sum;
    return (*this);
  }

  String &append(const char *str) {
    size_t size_sum = m_size + strlen(str);
    assert(size_sum <= MaxSize);
    size_t other_i = 0;
    for (size_t i = m_size; i < size_sum; ++i) {
      this->m_str[i] = str[other_i];
      ++other_i;
    }
    m_size = size_sum;
    return (*this);
  }

  char at(unsigned int idx) const {
    assert(idx < m_size);
    return m_str[idx];
  }

  // get functions
  const char *c_str() const {
    return m_str;
  }

  size_t getMaxSize() const {
    return MaxSize;
  }
  size_t getSize() const {
    return m_size;
  }

  String &operator=(const String &other) {
    this->m_size = other.m_size;
    memset(m_str, 0, MaxSize);
    for (size_t i = 0; i < this->m_size; ++i) {
      this->m_str[i] = other.m_str[i];
    }
    return (*this);
  }

  String &operator=(const char *str) {
    m_size = strlen(str);
    assert(m_size <= MaxSize);
    memset(m_str, 0, MaxSize);
    for (size_t i = 0; i < m_size; ++i) {
      m_str[i] = str[i];
    }
    return (*this);
  }

  String &operator+=(const String &other) {
    size_t size_sum = this->m_size + other.m_size;
    assert(size_sum <= MaxSize);
    size_t other_i = 0;
    for (size_t i = m_size; i < size_sum; ++i) {
      this->m_str[i] = other.m_str[other_i];
      ++other_i;
    }
    this->m_size = size_sum;
    return (*this);
  }

  String &operator+=(const char *str) {
    size_t size_sum = m_size + strlen(str);
    assert(size_sum <= MaxSize);
    size_t other_i = 0;
    for (size_t i = m_size; i < size_sum; ++i) {
      this->m_str[i] = str[other_i];
      ++other_i;
    }
    m_size = size_sum;
    return (*this);
  }

  char &operator[](Uint32 idx) {
    assert(idx < m_size);
    return m_str[idx];
  }

  const char &operator[](Uint32 idx) const {
    assert(idx < m_size);
    return m_str[idx];
  }

  String operator+(const String &other) {
    String result(*this);
    result += other;
    return result;
  }

  int32 operator==(const String &other) {
    if (this->m_size != other.m_size) {
      return false;
    } else {
      for (size_t i = 0; i < this->m_size; ++i) {
        if (this->m_str[i] != other.m_str[i]) {
          return false;
        }
      }
      return true;
    }
  }

  int32 operator!=(const String &other) {
    if (this->m_size != other.m_size) {
      return true;
    } else {
      for (size_t i = 0; i < this->m_size; ++i) {
        if (this->m_str[i] == other.m_str[i]) {
          return false;
        }
      }
      return true;
    }
  }

 protected:
  char m_str[MaxSize];
  size_t m_size;
};


// compile time hash function
// https://gist.github.com/Lee-R/3839813
namespace CompileTimeHash {
constexpr Uint32 val_const = 0x811c9dc5;
constexpr Uint32 prime_const = 0x1000193;
inline constexpr Uint32 fnv1a_32(const char *const str, size_t count = val_const) {
  return ((count ? fnv1a_32(str, count - 1) : val_const) ^ str[count]) * prime_const;
}

inline const Uint32 runtime_hash(const char *str, size_t count = val_const) {
  return ((count ? runtime_hash(str, count - 1) : val_const) ^ str[count]) * prime_const;
}
}

inline constexpr Uint32 operator ""_hash(const char *const str, size_t count) {
  return CompileTimeHash::fnv1a_32(str, count);
}

/////////////////////////////////////////////////////////

class StringID {
 private:
  StringID() = delete;

 public:
  explicit StringID(const char *str)
      : m_id(0) {
    m_id = CompileTimeHash::runtime_hash(str, strlen(str));
  }

  explicit StringID(const char *str, size_t length)
      : m_id(0) {
    m_id = CompileTimeHash::runtime_hash(str, length);
  }

  template<class StringT>
  explicit StringID(const StringT &str) {
    m_id = CompileTimeHash::runtime_hash(str.C_Str(), str.GetSize());
  }

  explicit StringID(const StringID &other)
      : m_id(other.m_id) {
    ;
  }

  ~StringID() {
    ;
  }

  explicit operator Uint32() const {
    return m_id;
  }

  int32 operator!=(const StringID &other) const {
    return ((this->m_id) != other.m_id);
  }

  int32 operator==(const StringID &other) const {
    return ((this->m_id) == other.m_id);
  }

  StringID &operator=(const StringID &other) {
    this->m_id = other.m_id;
    return (*this);
  }

 private:
  Uint32 m_id;
};

typedef String<16> String16;
typedef String<32> String32;
typedef String<48> String48;
typedef String<64> String64;
typedef String<128> String128;
}

#endif
