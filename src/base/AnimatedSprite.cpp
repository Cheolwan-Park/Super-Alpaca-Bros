#include "AnimatedSprite.hpp"
#include "Time.hpp"
#include "Application.hpp"

namespace Base
{
    // AnimatedSprite class
    AnimatedSprite::AnimatedSprite()
    :Sprite(), m_chage_remaintime(0.0f), m_nowidx(0), m_animation(nullptr)
    {
        ;
    }

    AnimatedSprite::AnimatedSprite(const AnimatedSprite &other)
    :Sprite(other), m_chage_remaintime(other.m_chage_remaintime), m_nowidx(other.m_nowidx), 
    m_animation(other.m_animation)
    {
        ;
    }

    AnimatedSprite::~AnimatedSprite()
    {
        ;
    }

    AnimatedSprite &AnimatedSprite::operator=(const AnimatedSprite &other)
    {
        assert(this != &other);
        Sprite::operator=(other);
        this->m_chage_remaintime = other.m_chage_remaintime;
        this->m_nowidx = other.m_nowidx;
        this->m_animation = other.m_animation;
        return (*this);
    }

    void AnimatedSprite::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        Sprite::InitWithJson(obj, allocator);

        assert(obj.HasMember("default_anim"));
        assert(obj["default_anim"].IsString());
        const char *defaultanim = obj["default_anim"].GetString();

        auto &animations = Application::Get().GetAnimationStorage();
        SetAnimation(animations[CompileTimeHash::runtime_hash(defaultanim, strlen(defaultanim))]);
    }

    void AnimatedSprite::Start()
    {
        Sprite::Start();
    }

    void AnimatedSprite::Update()
    {
        Sprite::Update();
        if(m_animation && isPlaying())
        {
            Time &t = Time::Get();
            m_chage_remaintime -= t.GetDeltatime();
            if(m_chage_remaintime < 0.0f)
            {
                ++m_nowidx;

                const Math::IRect &sprites_range = m_animation->GetSpritesRange();
                const Math::Size &sprites_count = m_animation->GetSpritesCount();

                if(m_nowidx >= sprites_count.w*sprites_count.h)
                {
                    if(m_animation->isLooping())
                    {
                        m_nowidx = 0;
                        SetAnimationUV();
                    }
                    else
                    {
                        m_flags.SetFlag(12, false);
                        OnFinish();
                    }
                }
                else
                    SetAnimationUV();
                m_chage_remaintime = m_animation->GetEachTime();
            }
        }
    }

    void AnimatedSprite::Release()
    {
        Sprite::Release();
    }

    int32 AnimatedSprite::isPlaying()const
    {
        return m_flags.GetFlag(10);
    }

    const Animation *AnimatedSprite::GetAnimation()const
    {
        return m_animation;
    }

    void AnimatedSprite::SetPlaying(int32 val)
    {
        m_flags.SetFlag(10, val);
    }

    void AnimatedSprite::SetAnimation(Animation *anim)
    {
        assert(anim);
        m_animation = anim;
        auto &textures = Application::Get().GetTextureStorage();
        SetTexture(textures[m_animation->GetTextureID()]);
        SetPlaying(true);
    }

    void AnimatedSprite::OnFinish()
    {
        ;
    }

    void AnimatedSprite::SetAnimationUV()
    {
        const Math::IRect &sprites_range = m_animation->GetSpritesRange();
        const Math::Size &sprites_count = m_animation->GetSpritesCount();
        int32 x=0, y=0;
        Math::IRect uv = {0,0,0,0};
        y = m_nowidx/sprites_count.w;
        x = m_nowidx%sprites_count.w;
        uv.w = sprites_range.w/sprites_count.w;
        uv.h = sprites_range.h/sprites_count.h;
        uv.x = sprites_range.x + x*uv.w;
        uv.y = sprites_range.y + y*uv.h;
        SetUV(uv);
    }
}