#ifndef BaseArray_hpp
#define BaseArray_hpp

#include "types.hpp"
#include <stdlib.h>
#include <string.h>

namespace Base {
template<class T>
class Array;

template<class T, size_t MaxSize>
class FixedArray;

template<class T>
class Array {
 public:
  typedef T ValueType;

 private:

 public:
  Array()
      : Array(32) {
    ;
  }

  explicit Array(size_t maxsize)
      : m_arr(nullptr), m_count(0), m_maxsize(maxsize) {
    m_arr = (ValueType *) malloc(sizeof(ValueType) * maxsize);
    assert(m_arr);
    memset(m_arr, 0, sizeof(ValueType) * maxsize);
  }

  Array(const Array &other)
      : m_arr(nullptr), m_count(other.m_count),
        m_maxsize(other.m_maxsize) {
    this->m_arr = (ValueType *) malloc(sizeof(ValueType) * m_maxsize);
    assert(this->m_arr);
    memcpy(this->m_arr, other.m_arr, sizeof(ValueType) * m_maxsize);
  }

  ~Array() {
    if (nullptr != m_arr) {
      free(m_arr);
      m_arr = nullptr;
    }
  }

  Array &operator=(const Array &other) {
    assert(this != &other);
    if (this->m_maxsize == other.m_maxsize) {
      memcpy(this->m_arr, other.m_arr, sizeof(ValueType) * m_maxsize);
    } else {
      ValueType *newarr = realloc(this->m_arr, sizeof(ValueType) * other.m_maxsize);
      assert(newarr);
      memcpy(newarr, other.m_arr, sizeof(ValueType) * other.m_maxsize);
      this->m_arr = newarr;
      this->m_maxsize = other.m_maxsize;
    }
    return (*this);
  }

  void grow() {
    size_t newsize = m_maxsize * 2;
    ValueType *newarr = (ValueType *) realloc(m_arr, sizeof(ValueType) * newsize);
    assert(newarr);
    ValueType *added = newarr + m_maxsize;
    memset((void *) added, 0, m_maxsize * sizeof(ValueType));
    m_arr = newarr;
    m_maxsize = newsize;
  }

  void clear() {
    memset(m_arr, 0, sizeof(ValueType) * m_maxsize);
    m_count = 0;
  }

  void append(ValueType val) {
    if (m_count >= m_maxsize)
      grow();
    m_arr[m_count] = val;
    ++m_count;
  }

  void append(ValueType *val, size_t len) {
    if (m_count + len >= m_maxsize)
      grow();
    memcpy(m_arr + m_count, val, len * sizeof(T));
    m_count += len;
  }

  ValueType &at(uint64_t idx) {
    assert(idx < m_maxsize);
    return m_arr[idx];
  }

  const ValueType &at(uint64_t idx) const {
    assert(idx < m_maxsize);
    return m_arr[idx];
  }

  ValueType &operator[](uint64_t idx) {
    assert(idx < m_maxsize);
    return m_arr[idx];
  }

  const ValueType &operator[](uint64_t idx) const {
    assert(idx < m_maxsize);
    return m_arr[idx];
  }

  const ValueType *getBuffer() const {
    return m_arr;
  }

  ValueType *getBuffer() {
    return m_arr;
  }

  size_t getCount() const {
    return m_count;
  }

  size_t getMaxSize() const {
    return m_maxsize;
  }

 private:
  ValueType *m_arr;
  size_t m_count;
  size_t m_maxsize;
};

template<class T, size_t MaxSize>
class FixedArray {
 public:
  typedef T ValueType;

 public:
  explicit FixedArray()
      : m_array() {
    ;
  }

  explicit FixedArray(const T *arr, size_t array_size)
      : m_array() {
    assert(array_size <= MaxSize);
    for (size_t i = 0; i < MaxSize; ++i) {
      m_array[i] = arr[i];
    }
  }

  FixedArray(const FixedArray &other)
      : m_array() {
    memcpy((void *) this->m_array, (void *) other.m_array, sizeof(ValueType) * MaxSize);
  }

  ~FixedArray() = default;

  FixedArray &operator=(const FixedArray &other) {
    assert(this != &other);
    memcpy((void *) this->m_array, (void *) other.m_array, sizeof(ValueType) * MaxSize);
    return (*this);
  }

  void fill(const ValueType &value) {
    for (size_t i = 0; i < MaxSize; ++i) {
      m_array[i] = value;
    }
  }

  void clear() {
    memset(m_array, 0, sizeof(m_array));
  }

  ValueType &at(uint64_t idx) {
    assert(idx < MaxSize);
    return m_array[idx];
  }

  const ValueType &at(uint64_t idx) const {
    assert(idx < MaxSize);
    return m_array[idx];
  }

  ValueType &operator[](uint64_t idx) {
    assert(idx < MaxSize);
    return m_array[idx];
  }

  const ValueType &operator[](uint64_t idx) const {
    assert(idx < MaxSize);
    return m_array[idx];
  }

  const ValueType *getBuffer() const {
    return m_array;
  }

  size_t getMaxSize() const {
    return MaxSize;
  }

 private:
  ValueType m_array[MaxSize];
};
}

#endif























