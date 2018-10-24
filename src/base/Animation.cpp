#include "Animation.hpp"

namespace Base
{
    // Animation class

    Animation::Animation()
    :m_sprites_range({0,0,0,0}), m_sprites_count({0,0}), 
    m_textureid(0), m_eachtime(0.0f), m_looping(false)
    {
        ;
    }

    Animation::Animation(const Animation &other)
    :m_sprites_range(other.m_sprites_range), 
    m_sprites_count(other.m_sprites_count),
    m_textureid(other.m_textureid),
    m_eachtime(other.m_eachtime),
    m_looping(other.m_looping)
    {
        ;
    }

    Animation::~Animation()
    {
        ;
    }

    Animation &Animation::operator=(const Animation &other)
    {
        assert(this != &other);
        this->m_sprites_range = other.m_sprites_range;
        this->m_sprites_count = other.m_sprites_count;
        this->m_textureid = other.m_textureid;
        this->m_eachtime = other.m_eachtime;
        this->m_looping = other.m_looping;
        return (*this);
    }

    const Math::IRect &Animation::GetSpritesRange()const
    {
        return m_sprites_range;
    }

    const Math::Size &Animation::GetSpritesCount()const
    {
        return m_sprites_count;
    }

    Uint32 Animation::GetTextureID()const
    {
        return m_textureid;
    }

    float32 Animation::GetEachTime()const
    {
        return m_eachtime;
    }

    int32 Animation::isLooping()const
    {
        return m_looping;
    }

    void Animation::SetSpritesRange(const Math::IRect &range)
    {
        m_sprites_range = range;
    }

    void Animation::SetSpritesCount(const Math::Size &count)
    {
        m_sprites_count = count;
    }

    void Animation::SetTextureID(Uint32 id)
    {
        m_textureid = id;
    }

    void Animation::SetEachTime(float32 t)
    {
        m_eachtime = t;
    }

    void Animation::SetLooping(int32 val)
    {
        m_looping = val;
    }

    void FreeAnimation(Animation **anim)
    {
        (*anim)->~Animation();
        (*anim) = nullptr;
    }
}