#ifndef GameAlpacaSpit_hpp
#define GameAlpacaSpit_hpp

#include "base.hpp"

using namespace Base;

namespace Game
{
    namespace Alpaca
    {
        class Alpaca;
        
        class Spit: public Component
        {
        public:
            COMPONENT(Spit);

            Spit();

            Spit(const Spit &other);

            virtual ~Spit();

            Spit &operator=(const Spit &other);

            virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

            virtual void Start();

            virtual void Update();

            virtual void Release();

            virtual void OnTriggerEnter(Collider *other);

            void Shoot(const glm::vec3 &direction, Alpaca *owner);

            int32 isShooted()const;

        private:
            float32 m_elapsedtime;
            float32 m_lifetime;
            float32 m_speed;
            float32 m_force;
            float32 m_guageup;
            Alpaca *m_owner;
            glm::vec3 m_direction;

            /*
             * flags
             * Component's flags
             * 2 : isShooted
             */
        };
    }
}

#endif