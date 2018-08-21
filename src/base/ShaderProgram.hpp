#ifndef BaseShaderProgram_hpp
#define BaseShaderProgram_hpp

#include "FileIO.hpp"
#include "types.hpp"
#ifdef _WIN32
#include <GL/gl.h>
#include <GL/glew.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

namespace Base
{
    using namespace Base;
    
    class ShaderProgram
    {
    public:
        ShaderProgram();
        
        ShaderProgram(const ShaderProgram &other) = delete;
        
        ~ShaderProgram();
        
        ShaderProgram &operator=(const ShaderProgram &other) = delete;
        
        int32 Init(const FileIO &vert, const FileIO &frag);
        
        void Release();
        
        GLuint GetProgram()const;
        
        GLuint GetViewProjectionLocation()const;
        
        GLuint GetTextureLocation()const;
        
    private:
        GLuint m_program;
        GLuint m_vert;
        GLuint m_frag;
        GLuint m_uniform_locs[2]; // 0 : vp, 1 : tex
    };
    
    class ShaderStorage
    {
    public:
        typedef ShaderProgram* Type;
        
        ShaderStorage();
        
        ShaderStorage(const ShaderStorage &other) = delete;
        
        ~ShaderStorage();
        
        ShaderStorage &operator=(const ShaderStorage &other) = delete;
        
        // len : length of array (not memory size)
        void AssignMemory(void *memory, Uint32 len);
        
        int32 Register(ShaderProgram *program, Uint32 hash);
        
        ShaderProgram *DeRegister(Uint32 hash);
        
        void Clear();
        
        const ShaderProgram *operator[](Uint32 hash)const;
        
    private:
        Uint32 m_len;
        ShaderProgram **m_shaders;
    };
}

#endif




