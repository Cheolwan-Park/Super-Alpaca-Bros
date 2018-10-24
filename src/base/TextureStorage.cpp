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

    void FreeTexture(Texture **tex)
    {
        if((*tex)->id)
            glDeleteTextures(1, &((*tex)->id));
        (*tex) = nullptr;
    }
}














