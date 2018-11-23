#ifndef BaseAllocator_hpp
#define BaseAllocator_hpp

#include "types.hpp"

namespace Base {
class StackAllocator {
 public:
  typedef uint64_t Marker;
 public:
  StackAllocator();

  explicit StackAllocator(size_t size);

  StackAllocator(const StackAllocator &other) = delete;

  ~StackAllocator();

  StackAllocator &operator=(const StackAllocator &other) = delete;

  // if failed to allocate, return nullptr
  void *allocMem(size_t size);
  template<typename T>
  inline T *alloc() { return (T *) allocMem(sizeof(T)); }
  template<typename T>
  inline T *alloc(size_t n) { return (T *) allocMem(sizeof(T) * n); }

  // if failed to free, return false
  bool freeWithMarker(Marker marker);

  // if failed to allocate, return false
  // if there is any chunk allocated, return false
  bool reallocBuffer(size_t size);

  // clear chunks
  void clear();

  // get functions
  Marker getTopMarker() const {
    return m_top;
  }

  size_t getMaxSize() const {
    return m_size;
  }

 private:
  Marker m_top;            // to mark top of stack
  size_t m_size;           // size of buffer
  void *m_buffer;          // memory buffer
};

template<class T>
class PoolAllocator {
 public:
  union Node {
    T data;
    Node *next;
  };
 public:
  PoolAllocator()
      : m_max_elements(0), m_mem_size(0),
        m_original_head(nullptr), m_head(nullptr), m_tail(nullptr) {
    ;
  }

  // allocate memory with specific StackAllocator object
  PoolAllocator(StackAllocator &allocator, size_t max_elements)
      : m_max_elements(max_elements), m_mem_size(max_elements * sizeof(Node)),
        m_head(nullptr), m_tail(nullptr) {
    reallocBuffer(allocator.allocMem(m_mem_size), max_elements);
  }

  PoolAllocator(void *mem, size_t max_elements)
      : m_max_elements(max_elements), m_mem_size(max_elements * sizeof(Node)),
        m_head(nullptr), m_tail(nullptr) {
    reallocBuffer(mem, max_elements);
  }

  ~PoolAllocator() {
    ;
  }

  // allocate memory for allocator
  void reallocBuffer(void *mem, size_t max_elements) {
    assert(mem);
    assert(max_elements);

    m_head = mem;
    m_max_elements = max_elements;
    m_mem_size = max_elements * sizeof(Node);

    // make list
    Node *iter = m_head;
    for (size_t i = 0; i < max_elements; ++i) {
      iter->next = (iter + 1);
      iter = iter->next;
    }
    m_tail = (m_head + max_elements);
    m_tail->next = nullptr;
  }

  T *alloc() {
    // check if there is enough memory
    assert(m_head);

    // allocate pointer
    T *result = &(m_head->data);
    m_head = m_head->next;
    return result;
  }

  void free(T *ptr) {
    Node *node = (Node *) ptr;
    // check if ptr was allocated by this allocator
    assert(m_original_head <= node && node <= (m_original_head + m_max_elements));

    // free pointer
    m_tail->next = node;
    node->next = nullptr;
  }

  // get functions
  size_t getNumMaxElements() const {
    return m_max_elements;
  }

  size_t getMemorySize() const {
    return m_mem_size;
  }

 protected:
  size_t m_max_elements;       // max number of elements
  size_t m_mem_size;           // size of pool(byte)
  Node *m_original_head;       // original head pos
  Node *m_head;                // head of list(pool)
  Node *m_tail;                // tail of list(pool)
};
}

#endif 
