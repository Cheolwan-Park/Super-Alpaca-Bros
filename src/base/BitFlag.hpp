#ifndef BaseBitFlag_hpp
#define BaseBitFlag_hpp

#include "types.hpp"

namespace Base {
class BitFlag {
 public:
  explicit BitFlag() : m_flag(0) { ; }

  BitFlag(const BitFlag &other) : m_flag(other.m_flag) { ; }

  ~BitFlag() { ; }

  BitFlag &operator=(const BitFlag &other) {
    assert(this != &other);
    this->m_flag = other.m_flag;
    return (*this);
  }

  int32_t getFlag(uint32_t idx) const {
    assert(idx < 32);
    return (m_flag >> idx) & 1U;
  }

  void setFlag(uint32_t idx, int32_t value) {
    assert(idx < 32);
    if (value)
      m_flag = m_flag | (1U << idx);
    else
      m_flag = m_flag & (~(1U << idx));
  }

  void reverseFlag(uint32_t idx) {
    assert(idx < 32);
    m_flag = m_flag ^ (1 << idx);
  }

  // count 1 bit in Uint (in the book, hackers delight)
  uint32_t countUpBit() const
  {
    uint32_t tmp = m_flag;
    tmp = tmp - ((tmp >> 1) & 0x55555555);
    tmp = (tmp & 0x33333333) + ((tmp >> 2) & 0x33333333);
    tmp = tmp + (tmp >> 8);
    tmp = tmp + (tmp >> 16);
    return tmp & 0x0000003F;
  }

  uint32_t countDownBit() const {
    return (32 - countUpBit());
  }

  uint32_t getUIntValue() const {
    return m_flag;
  }

  void printBits() {
    for (uint32_t i = 0; i < 32; ++i) {
      printf("%s", getFlag(31 - i) ? "1" : "0");
    }
    printf("\n");
  }

 private:
  uint32_t m_flag;
};
}

#endif
