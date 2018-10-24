#ifndef BaseArray_hpp
#define BaseArray_hpp

#include "types.hpp"
#include <stdlib.h>
#include <string.h>

namespace Base
{
    template <class T>
    class Array;
    
    template <class T, size_t MaxSize>
    class FixedArray;
    
    template <class T>
    class Array
    {
    public:
        typedef T ValueType;
        
    private:
        
    public:
        explicit Array()
        :Array(32,32)
        {
            ;
        }
        
        explicit Array(size_t maxsize, size_t growsize)
        :m_arr(nullptr), m_count(0), m_maxsize(maxsize), m_growsize(growsize)
        {
            m_arr = (ValueType*)malloc(sizeof(ValueType)*maxsize);
            assert(m_arr);
            memset(m_arr, 0, sizeof(ValueType)*maxsize);
        }
        
        Array(const Array &other)
        :m_arr(nullptr), m_count(other.m_count), 
        m_maxsize(other.m_maxsize), m_growsize(other.m_growsize)
        {
            this->m_arr = (ValueType*)malloc(sizeof(ValueType)*m_maxsize);
            assert(this->m_arr);
            memcpy(this->m_arr, other.m_arr, sizeof(ValueType)*m_maxsize);
        }
        
        ~Array()
        {
            if(nullptr != m_arr)
            {
                free(m_arr);
                m_arr = nullptr;
            }
        }
        
        Array &operator=(const Array &other)
        {
            assert(this != &other);
            if(this->m_maxsize == other.m_maxsize)
            {
                memcpy(this->m_arr, other.m_arr, sizeof(ValueType)*m_maxsize);
            }
            else
            {
                ValueType *newarr = realloc(this->m_arr, sizeof(ValueType)*other.m_maxsize);
                assert(newarr);
                memcpy(newarr, other.m_arr, sizeof(ValueType)*other.m_maxsize);
                this->m_arr = newarr;
                this->m_maxsize = other.m_maxsize;
                this->m_growsize = other.m_growsize;
            }
            return (*this);
        }
        
        void Grow()
        {
            size_t newsize = m_maxsize+m_growsize;
            ValueType *newarr = (ValueType*)realloc(m_arr, sizeof(ValueType)*newsize);
            assert(newarr);
            ValueType *added = newarr + m_maxsize;
            memset((void*)added, 0, m_growsize*sizeof(T));
            m_arr = newarr;
            m_maxsize = newsize;
        }
        
        void Clear()
        {
            memset(m_arr, 0, sizeof(ValueType)*m_maxsize);
            m_count = 0;
        }
        
        void Append(ValueType val)
        {
            if(m_count >= m_maxsize)
                Grow();
            m_arr[m_count] = val;
            ++m_count;
        }
        
        void Append(ValueType val[], size_t len)
        {
            if(m_count + len >= m_maxsize)
                Grow();
            memcpy(m_arr+m_count, val, len*sizeof(T));
            m_count += len;
        }
        
        ValueType &At(Uint64 idx)
        {
            assert(idx<m_maxsize);
            return m_arr[idx];
        }
        
        const ValueType &At(Uint64 idx)const
        {
            assert(idx<m_maxsize);
            return m_arr[idx];
        }
        
        ValueType &operator[](Uint64 idx)
        {
            assert(idx<m_maxsize);
            return m_arr[idx];
        }
        
        const ValueType &operator[](Uint64 idx)const
        {
            assert(idx<m_maxsize);
            return m_arr[idx];
        }
        
        const ValueType *GetBuffer()const
        {
            return m_arr;
        }
        
        ValueType *GetBuffer()
        {
            return m_arr;
        }
        
        size_t GetCount()const
        {
            return m_count;
        }
        
        size_t GetMaxSize()const
        {
            return m_maxsize;
        }
        
        size_t GetGrowSize()const
        {
            return m_growsize;
        }
        
    private:
        ValueType   *m_arr;
        size_t      m_count;
        size_t      m_maxsize;
        size_t      m_growsize;
    };
    
    template <class T, size_t MaxSize>
    class FixedArray
    {
    public:
        typedef T ValueType;
        
    public:
        explicit FixedArray()
        :m_array()
        {
            ;
        }
        
        explicit FixedArray(const T *arr, size_t arrsize)
        :m_array()
        {
            assert(arrsize <= MaxSize);
            for(size_t i=0; i<MaxSize; ++i)
            {
                m_array[i] = arr[i];
            }
        }
        
        explicit FixedArray(const FixedArray &other)
        :m_array()
        {
            memcpy((void*)this->m_array, (void*)other.m_array, sizeof(ValueType) * MaxSize);
        }
        
        ~FixedArray()
        {
            
        }
        
        FixedArray &operator=(const FixedArray &other)
        {
            assert(this != &other);
            memcpy((void*)this->m_array, (void*)other.m_array, sizeof(ValueType) * MaxSize);
            return (*this);
        }
        
        void Fill(const ValueType &value)
        {
            for(size_t i=0; i<MaxSize; ++i)
            {
                m_array[i] = value;
            }
        }
        
        void Clear()
        {
            memset(m_array, 0, sizeof(m_array));
        }
        
        ValueType &At(Uint64 idx)
        {
            assert(idx<MaxSize);
            return m_array[idx];
        }
        
        const ValueType &At(Uint64 idx)const
        {
            assert(idx<MaxSize);
            return m_array[idx];
        }
        
        ValueType &operator[](Uint64 idx)
        {
            assert(idx<MaxSize);
            return m_array[idx];
        }
        
        const ValueType &operator[](Uint64 idx)const
        {
            assert(idx<MaxSize);
            return m_array[idx];
        }
        
        const ValueType *GetBuffer()const
        {
            return m_array;
        }
        
        size_t GetMaxSize()const
        {
            return MaxSize;
        }

    private:
        ValueType m_array[MaxSize];
    };
}


#endif























