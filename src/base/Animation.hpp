#ifndef BaseAnimation_hpp
#define BaseAnimation_hpp

#include "Math.hpp"

namespace Base
{
    class Animation
    {
    public:
        Animation();

        Animation(const Animation &other);

        ~Animation();

        Animation &operator=(const Animation &other);

        // get
        const Math::IRect &GetSpritesRange()const;

        const Math::Size &GetSpritesCount()const;

        Uint32 GetTextureID()const;

        float32 GetEachTime()const;

        int32 isLooping()const;

        // set
        void SetSpritesRange(const Math::IRect &range);

        void SetSpritesCount(const Math::Size &count);

        void SetTextureID(Uint32 id);

        void SetEachTime(float32 t);

        void SetLooping(int32 val);

    private:
        Math::IRect m_sprites_range;
        Math::Size m_sprites_count;
        Uint32 m_textureid;
        float32 m_eachtime;
        int32 m_looping;
    };

    void FreeAnimation(Animation **anim);
}

#endif 