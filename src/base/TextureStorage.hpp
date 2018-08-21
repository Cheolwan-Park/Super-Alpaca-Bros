#ifndef BaseTextureStorage_hpp
#define BaseTextureStorage_hpp

#include "types.hpp"
#include "SDL.hpp"

namespace Base
{
    GLuint LoadTexture(const char *filename, GLint *w, GLint *h);
    
    class TextureStorage
    {
    public:
        struct Val
        {
            GLuint id;
            GLint w;
            GLint h;
        };
        
        typedef Val* Type;
        
        TextureStorage();
        
        TextureStorage(const TextureStorage &other) = delete;
        
        ~TextureStorage();
        
        TextureStorage &operator=(const TextureStorage &other) = delete;
        
        void AssignMemory(void *memory, Uint32 len);
        
        int32 Register(Val *tex, Uint32 hash);
        
        GLuint DeRegister(Uint32 hash);
        
        void Clear();
        
        const Val *operator[](Uint32 hash)const;
        
    private:
        Uint32 m_len;
        Val **m_textures;
    };
}

#endif
