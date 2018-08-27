#include "FileIO.hpp"
#include <stdio.h>

namespace Base
{
    FileIO::FileIO()
    :m_buffer(nullptr), m_size(0)
    {
        ;
    }
    
    FileIO::FileIO(FILE *file, void *memory, size_t size)
    :m_buffer(nullptr), m_size(0)
    {
        Open(file, memory, size);
    }
    
    FileIO::~FileIO()
    {
        // memory will be freed by other object
    }
    
    void FileIO::Open(void *memory, size_t size)
    {
        assert(nullptr != memory);
        assert(0 != size);
        m_buffer = (byte*)memory;
        m_size = size;
    }
    
    int32 FileIO::Open(FILE *file, void *memory, size_t size)
    {
        assert(file != nullptr);
        
        Open(memory, size);
        
        if(m_size != fread(m_buffer, sizeof(char), m_size, file))
            return RET_FAILED;

        return RET_SUCC;
    }
    
    int32 FileIO::Write(FILE *file)const
    {
        assert(nullptr != file);
        assert(IsAvailable());
        
        if(m_size != fwrite((void*)m_buffer, sizeof(char), m_size, file))
            return RET_FAILED;
        
        return RET_SUCC;
    }
    
    int32 FileIO::IsAvailable()const
    {
        return (nullptr != m_buffer && 0 != m_size);
    }
    
    byte *FileIO::GetBuffer()
    {
        return m_buffer;
    }
    
    const byte *FileIO::GetBuffer()const
    {
        return m_buffer;
    }
    
    const byte *const *FileIO::GetBufferPointer()const
    {
        return &m_buffer;
    }
    
    size_t FileIO::GetSize()const
    {
        return m_size;
    }
    
    
    // relative functions
    FILE *OpenFile(const char *filename, const char *mode)
    {
        FILE *result = nullptr;
        result = fopen(filename, mode);
        assert(result);
        return result;
    }
    
    size_t GetFileSize(FILE *file)
    {
        size_t result = 0;
        fseek(file, 0, SEEK_END);
        result = ftell(file);
        rewind(file);
        return result;
    }
}















