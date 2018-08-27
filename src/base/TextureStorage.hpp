#ifndef BaseTextureStorage_hpp
#define BaseTextureStorage_hpp

#include "types.hpp"
#include "SDL.hpp"

namespace Base
{
    GLuint LoadTexture(const char *filename, GLint *w, GLint *h);
    
    struct Texture
    {
        GLuint id;
        GLint w;
        GLint h;
    };

    void FreeTexture(Texture **tex);
}

#endif
