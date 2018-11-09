#ifndef BaseShaderProgram_hpp
#define BaseShaderProgram_hpp

#include "types.hpp"
#include "FileIO.hpp"
#include "Json.hpp"
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

        int32 InitWithJson(const rapidjson::Value::Object &obj, void *vertmem, void *fragmem);
        
        void Release();

        Uint32 GetID()const;
        
        GLuint GetProgram()const;
        
        GLuint GetViewProjectionLocation()const;
        
        GLuint GetTextureLocation()const;
        
    private:
        Uint32 m_id;
        GLuint m_program;
        GLuint m_vert;
        GLuint m_frag;
        GLuint m_uniform_locs[2]; // 0 : vp, 1 : tex
    };

    void FreeShader(ShaderProgram **prog);
}

#endif





