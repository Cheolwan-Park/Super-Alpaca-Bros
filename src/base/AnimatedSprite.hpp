#ifndef BaseAnimatedSprite_hpp
#define BaseAnimatedSprite_hpp

#include "Sprite.hpp"
#include "Math.hpp"
#include "Array.hpp"

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

    class AnimatedSprite : public Sprite
    {
    protected:
        static Storage<Animation> &GetAnimationsStorage();

    public:
        MAKE_TYPE_ID(AnimatedSprite);

        AnimatedSprite() = delete;

        AnimatedSprite(Uint32 id, int32 isStatic = false);
        
        AnimatedSprite(Uint32 id, const GameObject *parent, int32 isStatic = false);

        AnimatedSprite(const rapidjson::GenericObject<true, rapidjson::Value> &obj);
        
        AnimatedSprite(const Sprite &other);

        virtual ~AnimatedSprite();
        
        AnimatedSprite &operator=(const AnimatedSprite &other);

        virtual void Start();
        
        virtual void Update();

        // get
        int32 isPlaying()const;

        const Animation *GetAnimation()const;

        // set
        void SetPlaying(int32 val);

        void SetAnimation(Uint32 hash);

    private:
        virtual void OnFinish();

    private:
        float32 m_chage_remaintime;
        int32 m_nowidx;
        const Animation *m_animation;
        /* flags
         * GameObject's flags
         * Sprite's flags
         * 12 : playing animation
         */
    };
}


#endif