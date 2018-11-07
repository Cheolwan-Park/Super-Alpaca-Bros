#include "AlpacaSpit.hpp"
#include "Alpaca.hpp"

namespace Game
{
    namespace Alpaca
    {
        Spit::Spit()
        :Component(), m_elapsedtime(0.0f), m_lifetime(0.0f), 
        m_speed(0.0f), m_force(0.0f), m_guageup(0.0f), 
        m_owner(nullptr), m_direction()
        {
            ;
        }

        Spit::Spit(const Spit &other)
        :Component(other), m_elapsedtime(0.0f), m_lifetime(other.m_lifetime), 
        m_speed(other.m_speed), m_force(other.m_force), m_guageup(other.m_guageup), 
        m_owner(other.m_owner), m_direction(other.m_direction)
        {
            ;
        }

        Spit::~Spit()
        {
            ;
        }

        Spit &Spit::operator=(const Spit &other)
        {
            assert(this != &other);
            Component::operator=(other);
            this->m_lifetime = other.m_lifetime;
            this->m_speed = other.m_speed;
            this->m_force = other.m_force;
            this->m_guageup = other.m_guageup;
            this->m_owner = other.m_owner;
            this->m_direction = other.m_direction;
            return (*this);
        }

        void Spit::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
        {
            Component::InitWithJson(obj, allocator);
            
            assert(obj.HasMember("lifetime"));
            assert(obj.HasMember("speed"));
            assert(obj.HasMember("force"));
            assert(obj.HasMember("gaugeup"));
            assert(obj["lifetime"].IsFloat());
            assert(obj["speed"].IsFloat());
            assert(obj["force"].IsFloat());
            assert(obj["gaugeup"].IsFloat());

            m_lifetime = obj["lifetime"].GetFloat();
            m_speed = obj["speed"].GetFloat();
            m_force = obj["force"].GetFloat();
            m_guageup = obj["gaugeup"].GetFloat();
        }

        void Spit::Start()
        {
            Component::Start();
        }

        void Spit::Update()
        {
            if(isShooted())
            {
                Time &t = Time::Get();
                Move(m_direction * (float32)(m_speed * t.GetDeltatime()));

                m_elapsedtime += t.GetDeltatime();
                if(m_elapsedtime > m_lifetime)
                {
                    GetGameObject()->SetAvailable(false);
                    m_flags.SetFlag(2, false);
                }
            }
        }

        void Spit::Release()
        {
            ;
        }

        void Spit::OnTriggerEnter(Collider *other)
        {
            if((other->GetTag()) == "player"_hash 
            && (other->GetGameObject()) != (m_owner->GetGameObject()))
            {
                Alpaca *alpaca = other->GetGameObject()->GetComponent<Alpaca>();
                Rigidbody *rigid = alpaca->GetRigidbody();
                HitGauge *hitgauge = alpaca->GetHitGauge();

                float32 force = m_force * (hitgauge->GetFactor());
                rigid->AddForce(m_direction*force);
                hitgauge->GaugeUp(m_guageup);

                GetGameObject()->SetAvailable(false);
            }
        }

        void Spit::Shoot(const glm::vec3 &direction, Alpaca *owner)
        {
            m_direction = glm::normalize(direction);
            m_owner = owner;
            m_elapsedtime = 0.0f;
            m_flags.SetFlag(2, true);
        }

        int32 Spit::isShooted()const
        {
            return m_flags.GetFlag(2);
        }
    }
}