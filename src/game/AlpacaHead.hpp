#ifndef GameHead_hpp
#define GameHead_hpp

#include "base.hpp"

using namespace Base;

namespace Game
{
    namespace Alpaca
    {
        class Alpaca;

        class Head : public Sprite
        {
        public:
            static constexpr Uint32 NECK_COUNT = 18;

        public:
            COMPONENT(Head);

            Head();

            Head(const Head &other) = delete;

            virtual ~Head();

            Head &operator=(const Head &other) = delete;

            virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

            virtual void Start();

            virtual void Update();

            virtual void Release();

            virtual void Draw();

            void ResetPosition();

            // set
            void SetAlpaca(Alpaca *alpaca);

            void SetHeadPos(const glm::vec3 &pos);

            // get
            GameObject &GetNeck(int32 i);

            Uint32 GetNeckCount()const;

            const glm::vec3 &GetHeadPos()const;

            const glm::vec3 &GetNeckPos()const;

        private:
            Alpaca *m_alpaca;
            FixedArray<GameObject, NECK_COUNT> m_necks;
            FixedArray<Sprite*, NECK_COUNT> m_necksprites; 
            glm::vec3 m_headpos;
            glm::vec3 m_neckpos;
            /* flags
            * Sprite's flags
            */
        };
    }
}

#endif