#ifndef BaseStorage_hpp
#define BaseStorage_hpp

#include "types.hpp"

namespace Base {
template<typename T>
class Storage {
 public:
  typedef T* Type;
  typedef void (*Func)(Type *val);

  Storage()
      : m_len(0), m_data(nullptr) {
    ;
  }

  Storage(const Storage &other) = delete;

  ~Storage() {
    clear();
  }

  Storage &operator=(const Storage &other) = delete;

  void assignMemory(void *mem, uint32_t len) {
    m_len = len;
    m_data = (Type *) mem;
    memset(mem, 0, sizeof(Type) * m_len);
  }

  T *add(T *data, uint32_t hash) {
    assert(data);
    assert(m_data);

    uint32_t idx = hash % m_len;
    // already exist same hash
    assert(!m_data[idx]);
    m_data[idx] = data;
    return data;
  }

  void erase(uint32_t hash) {
    uint32_t idx = hash % m_len;
    if (nullptr != m_data[idx]) {
      T *result = m_data[idx];
      m_data[idx] = nullptr;
      if (m_free_func)
        m_free_func(&result);
    }
  }

  void clear() {
    for (int i = 0; i < m_len; ++i) {
      if (m_data[i]) {
        if (m_free_func)
          m_free_func(&m_data[i]);
        m_data[i] = nullptr;
      }
    }
  }

  void forDo(Func f) {
    assert(f);
    for (int i = 0; i < m_len; ++i) {
      if (m_data[i]) {
        f(&m_data[i]);
      }
    }
  }

  T *get(uint32_t hash) {
    return m_data[hash % m_len];
  }

  const T *get(uint32_t hash) const {
    return m_data[hash % m_len];
  }

  T *operator[](uint32_t hash) {
    return m_data[hash % m_len];
  }

  const T *operator[](uint32_t hash) const {
    return m_data[hash % m_len];
  }

  void setFreeFunc(Func f) {
    m_free_func = f;
  }

 private:
  uint32_t m_len;
  Type *m_data;
  Func m_free_func;
};
}

#endif