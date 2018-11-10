#ifndef BaseStorage_hpp
#define BaseStorage_hpp

#include "types.hpp"

namespace Base {
template<typename T>
class Storage {
 public:
  typedef T *Type;
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

  void assignMemory(void *mem, Uint32 len) {
    m_len = len;
    m_data = (Type *) mem;
    memset(mem, 0, sizeof(Type) * m_len);
  }

  T *add(T *data, Uint32 hash) {
    assert(data);
    assert(m_data);

    Uint32 idx = hash % m_len;
    // already exist same hash
    assert(!m_data[idx]);
    m_data[idx] = data;
    return data;
  }

  void erase(Uint32 hash) {
    Uint32 idx = hash % m_len;
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

  T *get(Uint32 hash) {
    return m_data[hash % m_len];
  }

  const T *get(Uint32 hash) const {
    return m_data[hash % m_len];
  }

  T *operator[](Uint32 hash) {
    return m_data[hash % m_len];
  }

  const T *operator[](Uint32 hash) const {
    return m_data[hash % m_len];
  }

  void setFreeFunc(Func f) {
    m_free_func = f;
  }

 private:
  Uint32 m_len;
  Type *m_data;
  Func m_free_func;
};
}

#endif