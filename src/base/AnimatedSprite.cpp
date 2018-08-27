#include "AnimatedSprite.hpp"
#include "Time.hpp"
#include "Application.hpp"

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



    // AnimatedSprite class

    Storage<Animation> &AnimatedSprite::GetAnimationsStorage()
    {
        static Storage<Animation> instance;
        return instance;
    }

    AnimatedSprite::AnimatedSprite(Uint32 id, int32 isStatic)
    :Sprite(id, isStatic), m_chage_remaintime(0.0f), m_nowidx(0),
    m_animation(nullptr)
    {
        ;
    }

    AnimatedSprite::AnimatedSprite(Uint32 id, const GameObject *parent, int32 isStatic)
    :Sprite(id, parent, isStatic), m_chage_remaintime(0.0f), m_nowidx(0), m_animation(nullptr)
    {
        ;
    }

    AnimatedSprite::AnimatedSprite(const rapidjson::GenericObject<true, rapidjson::Value> &obj)
    :Sprite(obj), m_chage_remaintime(0.0f), m_nowidx(0), m_animation(nullptr)
    {
        assert(obj.HasMember("AnimatedSprite"));
        assert(obj["AnimatedSprite"].IsObject());
        auto aobj = obj["AnimatedSprite"].GetObject();
        
        assert(aobj.HasMember("animations"));
        assert(aobj.HasMember("default"));
        assert(aobj["animations"].IsArray());
        assert(aobj["default"].IsString());
        auto loadlist = aobj["animations"].GetArray();
        const char *defaultanim = aobj["default"].GetString();
        
        auto &animations = GetAnimationsStorage();
        Application &app = Application::Get();
        auto &textures = app.GetTextureStorage();
        auto &allocator = app.GetAllocator();
        Animation *newanim = nullptr;
        for(auto itr = loadlist.begin();
            loadlist.end() != itr;
            ++itr)
            {
                assert(itr->IsObject());
                auto anim = itr->GetObject();
                assert(anim.HasMember("name"));
                assert(anim.HasMember("texture"));
                assert(anim.HasMember("range"));
                assert(anim.HasMember("count"));
                assert(anim.HasMember("eachtime"));
                assert(anim.HasMember("loop"));
                assert(anim["name"].IsString());
                assert(anim["texture"].IsString());
                assert(anim["range"].IsObject());
                assert(anim["count"].IsObject());
                assert(anim["eachtime"].IsFloat());
                assert(anim["loop"].IsBool());

                const char *name = anim["name"].GetString();
                const char *texture_id = anim["texture"].GetString();
                Math::IRect range = {0, 0, 0, 0};
                JsonParseMethods::ReadIRect(anim["range"].GetObject(), &range);
                Math::Size count = {0, 0};
                JsonParseMethods::ReadSize(anim["count"].GetObject(), &count);
                float32 eachtime = anim["eachtime"].GetFloat();
                int32 looping = anim["loop"].GetBool();

                newanim = new (allocator.Alloc<Animation>()) Animation();
                assert(newanim);
                newanim->SetSpritesRange(range);
                newanim->SetSpritesCount(count);
                newanim->SetTextureID(CompileTimeHash::runtime_hash(texture_id, strlen(texture_id)));
                newanim->SetEachTime(eachtime);
                newanim->SetLooping(looping);

                animations.Register(newanim, CompileTimeHash::runtime_hash(name, strlen(name)));
            }
        SetAnimation(CompileTimeHash::runtime_hash(defaultanim, strlen(defaultanim)));
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
        Sprite::operator=(other);
        this->m_chage_remaintime = other.m_chage_remaintime;
        this->m_nowidx = other.m_nowidx;
        this->m_animation = other.m_animation;
        return (*this);
    }

    void AnimatedSprite::Start()
    {
        Sprite::Start();
    }

    void AnimatedSprite::Update()
    {
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
                    OnFinish();
                    if(m_animation->isLooping())
                        m_nowidx = 0;
                    else
                        m_flags.SetFlag(12, false);
                }
                else
                {
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
                m_chage_remaintime = m_animation->GetEachTime();
            }
        }
    }

    int32 AnimatedSprite::isPlaying()const
    {
        return m_flags.GetFlag(12);
    }

    const Animation *AnimatedSprite::GetAnimation()const
    {
        return m_animation;
    }

    void AnimatedSprite::SetPlaying(int32 val)
    {
        m_flags.SetFlag(12, val);
    }

    void AnimatedSprite::SetAnimation(Uint32 hash)
    {
        auto &animations = GetAnimationsStorage();
        auto &textures = Application::Get().GetTextureStorage();
        m_animation = animations[hash];
        assert(m_animation);
        SetTexture(textures[m_animation->GetTextureID()]);
    }

    void AnimatedSprite::OnFinish()
    {
        ;
    }
}