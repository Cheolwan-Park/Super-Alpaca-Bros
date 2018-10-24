#ifndef BaseAnimatedSprite_hpp
#define BaseAnimatedSprite_hpp

#include "Sprite.hpp"
#include "Math.hpp"
#include "Animation.hpp"

namespace Base
{
    class AnimatedSprite : public Sprite
    {
    public:
        COMPONENT(AnimatedSprite);

        AnimatedSprite();
        
        AnimatedSprite(const AnimatedSprite &other);

        virtual ~AnimatedSprite();
        
        AnimatedSprite &operator=(const AnimatedSprite &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual void Start();
        
        virtual void Update();

        virtual void Release();

        // get
        int32 isPlaying()const;

        const Animation *GetAnimation()const;

        // set
        void SetPlaying(int32 val);

        void SetAnimation(Animation *anim);

    private:
        virtual void OnFinish();
        void SetAnimationUV();

    private:
        float32 m_chage_remaintime;
        int32 m_nowidx;
        const Animation *m_animation;
        /* flags
         * Sprite's flags
         * 10 : playing animation
         */
    };
}


#endif