#include "TextureStorage.hpp"
#include "SDL.hpp"
#include <png.h>
#include <string.h>

namespace Base
{
    GLuint LoadTexture(const char *filename, GLint *w, GLint *h)
    {
        SDL_Surface *surface = IMG_Load(filename);
        SDL_Surface *formated = nullptr;
        GLuint result = 0;

        assert(surface);
        if(SDL_PIXELFORMAT_RGBA32 != surface->format->format)
        {
            formated = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
            SDL_FreeSurface(surface);
            surface = nullptr;
        }
        else
        {
            formated = surface;
        }

        *w = formated->w;
        *h = formated->h;
        glGenTextures(1, &result);
        glBindTexture(GL_TEXTURE_2D, result);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                    formated->w, formated->h, 0, 
                    GL_RGBA, GL_UNSIGNED_BYTE, formated->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        SDL_FreeSurface(formated);
        return result;
    }
    
    
    // IMGStorage class
    
    TextureStorage::TextureStorage()
    :m_len(0), m_textures(nullptr)
    {
        ;
    }
    
    TextureStorage::~TextureStorage()
    {
        Clear();
    }
    
    void TextureStorage::AssignMemory(void *memory, Uint32 len)
    {
        m_len = len;
        m_textures = (TextureStorage::Val**)memory;
        memset(m_textures, 0, sizeof(Val*)*m_len);
    }
    
    int32 TextureStorage::Register(TextureStorage::Val *tex, Uint32 hash)
    {
        assert(m_textures);
        assert(tex);
        assert(tex->id);
        
        Uint32 idx = hash%m_len;
        
        if(nullptr != m_textures[idx])
            return RET_FAILED;
        
        m_textures[idx] = tex;
        return RET_SUCC;
    }
    
    GLuint TextureStorage::DeRegister(Uint32 hash)
    {
        Uint32 idx = hash%m_len;
        if(nullptr != m_textures[idx])
        {
            GLuint result = m_textures[idx]->id;
            m_textures[idx] = nullptr;
            return result;
        }
        else
            return 0;
    }
    
    void TextureStorage::Clear()
    {
        if(nullptr != m_textures)
        {
            for(Uint32 i=0; i<m_len; ++i)
            {
                if(nullptr != m_textures[i])
                {
                    if(0 != m_textures[i]->id)
                        glDeleteTextures(1, &(m_textures[i]->id));
                    m_textures[i] = nullptr;
                }
            }
        }
    }
    
    const TextureStorage::Val *TextureStorage::operator[](Uint32 hash)const
    {
        Uint32 idx = hash%m_len;
        return m_textures[idx];
    }
}














