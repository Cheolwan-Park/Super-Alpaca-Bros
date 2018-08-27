#include "Allocator.hpp"
#include <cstdlib>
#include <cstring>

namespace Base
{
    // StackAllocator class
    
    StackAllocator::StackAllocator()
    : m_top(0), m_size(0), m_buffer(nullptr)
    {
        ;
    }
    
    StackAllocator::StackAllocator(const size_t size)
    :m_top(0), m_size(size), m_buffer(nullptr)
    {
        m_buffer = malloc(size);
        assert(m_buffer);
    }
    
    StackAllocator::~StackAllocator()
    {
        if(nullptr != m_buffer)
        {
            free(m_buffer);
        }
    }
    
    void *StackAllocator::Alloc(const size_t size)
    {
        // check if allocator can allocate memory offered
        assert(m_top + size <= m_size);
        
        // allocate memory for chunk
        void *allocated_memory = (void*)((char*)m_buffer + m_top);
        memset(allocated_memory, 0, size);
        m_top += size;
        
        return allocated_memory;
    }
    
    bool StackAllocator::FreeWithMarker(Marker marker)
    {
        // check memory marker marks is allocated
        assert(m_top > marker);
        
        // free memory
        m_top = marker;
        
        return true;
    }
    
    bool StackAllocator::ReAllocBuffer(const size_t size)
    {
        // reallocate buffer
        m_buffer = realloc(m_buffer, size);
        assert(m_buffer);
        m_size = size;
        
        return true;
    }
    
    void StackAllocator::Clear()
    {
        m_top = 0;
    }
}



