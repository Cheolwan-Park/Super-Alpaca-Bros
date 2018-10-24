#ifndef BaseStorage_hpp
#define BaseStorage_hpp

#include "types.hpp" 

namespace Base 
{
    template <typename T>
    class Storage
    {
    public:
        typedef T* Type;
        typedef void (*Func)(Type *val);

        Storage()
        :m_len(0), m_datas(nullptr)
        {
            ;
        }

        Storage(const Storage &other) = delete;

        ~Storage()
        {
            Clear();
        }

        Storage &operator=(const Storage &other) = delete;

        void AssignMemory(void *mem, Uint32 len)
        {
            m_len = len;
            m_datas = (Type*)mem;
            memset(mem, 0, sizeof(Type)*m_len);
        }

        T *Register(T* data, Uint32 hash)
        {
            assert(data);
            assert(m_datas);
            
            Uint32 idx = hash%m_len;
            // already exist same hash
            assert(!m_datas[idx]);  
            m_datas[idx] = data;
            return data;
        }

        void DeRegister(Uint32 hash)
        {
            Uint32 idx = hash%m_len;
            if(nullptr != m_datas[idx])
            {
                T *result = m_datas[idx];
                m_datas[idx] = nullptr;
                if(m_freefunc)
                    m_freefunc(&result);
            }
        }

        void Clear()
        {
            for(int i=0; i<m_len; ++i)
            {
                if(m_datas[i])
                {
                    if(m_freefunc)
                        m_freefunc(&m_datas[i]);
                    m_datas[i] = nullptr;
                }
            }
        }

        void ForDo(Func f)
        {
            assert(f);
            for(int i=0; i<m_len; ++i)
            {
                if(m_datas[i])
                {
                    f(&m_datas[i]);
                }
            }
        }

        T *Get(Uint32 hash)
        {
            return m_datas[hash%m_len];
        }

        const T *Get(Uint32 hash)const
        {
            return m_datas[hash%m_len];
        }

        T *operator[](Uint32 hash)
        {
            return m_datas[hash%m_len];
        }

        const T *operator[](Uint32 hash)const
        {
            return m_datas[hash%m_len];
        }

        void SetFreeFunc(Func f)
        {
            m_freefunc = f;
        }

    private:
        Uint32 m_len;
        Type *m_datas;
        Func m_freefunc;
    };
}

#endif