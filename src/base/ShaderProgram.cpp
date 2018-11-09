#include "ShaderProgram.hpp"

namespace Base
{
    // ShaderProgram class
    
    ShaderProgram::ShaderProgram()
    :m_id(0), m_program(0), m_vert(0), m_frag(0), m_uniform_locs()
    {
        ;
    }
    
    ShaderProgram::~ShaderProgram()
    {
        Release();
    }
    
    int32 ShaderProgram::Init(const FileIO &vert, const FileIO &frag)
    {
        assert(0 == m_program);
        assert(vert.IsAvailable());
        assert(frag.IsAvailable());
        
        int32 status_vert=GL_TRUE, status_frag=GL_TRUE, status_link=GL_TRUE;
        int32 log_len = 0;
        
        // compile vertex shader
        m_vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_vert, 1, vert.GetBufferPointer(), 0);
        glCompileShader(m_vert);
        
        glGetShaderiv(m_vert, GL_COMPILE_STATUS, &status_vert);
        if(GL_FALSE == status_vert)
        {
            glGetShaderiv(m_vert, GL_INFO_LOG_LENGTH, &log_len);
            GLchar *info = (GLchar*)malloc(log_len);
            glGetShaderInfoLog(m_vert, log_len, &log_len, info);
            fprintf(stderr, "compiling vertex shader failed, info:\n%s", info);
            free(info);
            return RET_FAILED;
        }
        
        // compile fragment shader
        m_frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_frag, 1, frag.GetBufferPointer(), 0);
        glCompileShader(m_frag);
        
        glGetShaderiv(m_frag, GL_COMPILE_STATUS, &status_frag);
        if(GL_FALSE == status_frag)
        {
            glGetShaderiv(m_frag, GL_INFO_LOG_LENGTH, &log_len);
            GLchar *info = (GLchar*)malloc(log_len);
            glGetShaderInfoLog(m_frag, log_len, &log_len, info);
            fprintf(stderr, "compiling fragment shader failed, info:\n%s", info);
            free(info);
            glDeleteShader(m_vert);
            m_vert = m_frag = 0;
            return RET_FAILED;
        }
        
        // link shader
        m_program = glCreateProgram();
        glAttachShader(m_program, m_vert);
        glAttachShader(m_program, m_frag);
        glLinkProgram(m_program);
        
        glGetProgramiv(m_program, GL_LINK_STATUS, &status_link);
        if(GL_FALSE == status_link)
        {
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_len);
            GLchar *info = (GLchar*)malloc(log_len);
            glGetProgramInfoLog(m_program, log_len, &log_len, info);
            fprintf(stderr, "link shaders failed, info:\n%s", info);
            free(info);
            glDeleteShader(m_vert);
            glDeleteShader(m_frag);
            m_program = m_vert = m_frag = 0;
            return RET_FAILED;
        }
        
        m_uniform_locs[0] = glGetUniformLocation(m_program, "vp");
        m_uniform_locs[1] = glGetUniformLocation(m_program, "tex");
        
        return RET_SUCC;
    }

    int32 ShaderProgram::InitWithJson(const rapidjson::Value::Object &obj, void *vertmem, void *fragmem)
    {
        assert(obj.HasMember("name"));
        assert(obj.HasMember("vert"));
        assert(obj.HasMember("frag"));
        assert(obj["name"].IsString());
        assert(obj["vert"].IsString());
        assert(obj["frag"].IsString());

        const char *shader_name = obj["name"].GetString();
        StringID id(shader_name);
        m_id = (Uint32)id;

        // open shader files
        const char *vert_filename = obj["vert"].GetString();
        const char *frag_filename = obj["frag"].GetString();
        FILE *vert = nullptr, *frag = nullptr;
        size_t size_vert = 0, size_frag = 0;
        FileIO vert_io, frag_io;

        String128 path = Directories::Shader;
        path += vert_filename;
        vert = OpenFile(path.C_Str(), "r");

        path.Clear();
        path = Directories::Shader;
        path += frag_filename;
        frag = OpenFile(path.C_Str(), "r");

        size_vert = GetFileSize(vert);
        size_frag = GetFileSize(frag);

        if(nullptr == vertmem && nullptr == fragmem)
        {
            vertmem = malloc(size_vert+1);
            fragmem = malloc(size_frag+1);
        }
        else
        {
            vertmem = realloc(vertmem, size_vert+1);
            fragmem = realloc(fragmem, size_frag+1);
        }
        memset(vertmem, 0, size_vert+1);
        memset(fragmem, 0, size_frag+1);

        if(RET_SUCC != vert_io.Open(vert, vertmem, size_vert))
        {
            return RET_FAILED;
        }

        if(RET_SUCC != frag_io.Open(frag, fragmem, size_frag))
        {
            return RET_FAILED;
        }

        if(RET_SUCC != Init(vert_io, frag_io))
        {
            return RET_FAILED;
        }

        return RET_SUCC;
    }
    
    void ShaderProgram::Release()
    {
        if(0 != m_program)
        {
            glDeleteShader(m_vert);
            glDeleteShader(m_frag);
            glDeleteProgram(m_program);
            m_vert = m_frag = m_program = 0;
        }
    }
    
    Uint32 ShaderProgram::GetID()const
    {
        return m_id;
    }

    GLuint ShaderProgram::GetProgram()const
    {
        return m_program;
    }
    
    GLuint ShaderProgram::GetViewProjectionLocation()const
    {
        return m_uniform_locs[0];
    }
    
    GLuint ShaderProgram::GetTextureLocation()const
    {
        return m_uniform_locs[1];
    }

    void FreeShader(ShaderProgram **prog)
    {
        (*prog)->~ShaderProgram();
        (*prog) = nullptr;
    }
}










