#ifndef BaseBitFlag_hpp
#define BaseBitFlag_hpp

#include "types.hpp"

namespace Base
{
    class BitFlag
    {
    public:
        explicit BitFlag() : m_flag(0) { ; }
        
        BitFlag(const BitFlag &other) : m_flag(other.m_flag) { ; }
        
        ~BitFlag() { ; }
        
        BitFlag &operator=(const BitFlag &other)
        {
            assert(this != &other);
            this->m_flag = other.m_flag;
            return (*this);
        }
        
        int32 GetFlag(Uint32 idx)const
        {
            assert(idx < 32);
            return (m_flag>>idx) & 1UL;
        }
        
        void SetFlag(Uint32 idx, int32 value)
        {
            assert(idx < 32);
            if(true == value)
                m_flag = m_flag | (1U << idx);
            else
                m_flag = m_flag & (~(1UL << idx));
        }
        
        void ReverseFlag(Uint32 idx)
        {
            assert(idx < 32);
            m_flag = m_flag ^ (1 << idx);
        }
        
        Uint32 CountUpBit()const    // count 1 bit in Uint (in the book, hackers delight)
        {
            Uint32 tmp = m_flag;
            tmp = tmp - ((tmp >> 1) & 0x55555555);
            tmp = (tmp & 0x33333333) + ((tmp >> 2) & 0x33333333);
            tmp = tmp + (tmp >> 8);
            tmp = tmp + (tmp >> 16);
            return tmp & 0x0000003F;
        }
        
        Uint32 CountDownBit()const
        {
            return (32 - CountUpBit());
        }
        
        Uint32 GetUIntValue()const
        {
            return m_flag;
        }
        
        void PrintBits()
        {
            for(Uint32 i=0; i<32; ++i)
            {
                printf("%s", GetFlag(31-i) ? "1" : "0");
            }
            printf("\n");
        }
        
    private:
        Uint32 m_flag;
    };
}


#endif 
