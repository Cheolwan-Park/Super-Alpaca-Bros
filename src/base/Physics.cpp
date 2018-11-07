#include "Physics.hpp"
#include "Collider.hpp"
#include "Time.hpp"

namespace Base
{
    // Physics class
    Physics::Physics()
    :m_collidercount(0), m_maxcollidercount(0), m_colliders(nullptr), m_collidemap(nullptr),
    m_gravity(0.0f)
    {
        ;
    }

    Physics::~Physics()
    {
        ;
    }

    void Physics::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        assert(obj.HasMember("collider_count"));
        assert(obj.HasMember("gravity"));
        assert(obj["collider_count"].IsInt());
        assert(obj["gravity"].IsFloat());

        m_maxcollidercount = obj["collider_count"].GetInt();
        m_gravity = obj["gravity"].GetFloat();
        
        m_colliders = allocator.Alloc<Collider*>(m_maxcollidercount);
        memset(m_colliders, 0, sizeof(Collider*)*m_maxcollidercount);
        m_collidemap = (byte*)allocator.Alloc<byte>(m_maxcollidercount*m_maxcollidercount);
        memset(m_collidemap, 0, sizeof(byte)*m_maxcollidercount*m_maxcollidercount);
    }

    void Physics::Update()
    {
        Time &t = Time::Get();
        Rigidbody *rigidbody = nullptr;

        byte *collidemap = m_collidemap;
        for(Uint32 i=0; i<m_maxcollidercount; ++i)
        {
            if(!m_colliders[i] 
            || !m_colliders[i]->isAvailable() 
            || !m_colliders[i]->GetGameObject()->isAvailable())
            {
                continue;
            }

            // apply gravity
            rigidbody = m_colliders[i]->GetRigidbody();
            if(rigidbody && rigidbody->isGravityActive())
            {
                rigidbody->Accelerate(0.0f, -m_gravity*t.GetDeltatime(), 0.0f);
            }

            // check collide
            for(Uint32 j=i+1; j<m_maxcollidercount; ++j)
            {
                if(!m_colliders[j] 
                || !m_colliders[j]->isAvailable()
                || !m_colliders[j]->GetGameObject()->isAvailable())
                {
                    continue;
                }
                if(m_colliders[i]->isCollideWith(m_colliders[j]))
                {
                    if(collidemap[j])
                    {
                        if(m_colliders[i]->isTrigger() || m_colliders[j]->isTrigger())
                        {
                            m_colliders[i]->GetGameObject()->OnTriggerStay(m_colliders[j]);
                            m_colliders[j]->GetGameObject()->OnTriggerStay(m_colliders[i]);
                        }
                        else 
                        {
                            m_colliders[i]->GetGameObject()->OnColliderStay(m_colliders[j]);
                            m_colliders[j]->GetGameObject()->OnColliderStay(m_colliders[i]);
                        } 
                    }
                    else
                    {
                        if(m_colliders[i]->isTrigger() || m_colliders[j]->isTrigger())
                        {
                            m_colliders[i]->GetGameObject()->OnTriggerEnter(m_colliders[j]);
                            m_colliders[j]->GetGameObject()->OnTriggerEnter(m_colliders[i]);
                        }
                        else 
                        {
                            m_colliders[i]->GetGameObject()->OnColliderEnter(m_colliders[j]);
                            m_colliders[j]->GetGameObject()->OnColliderEnter(m_colliders[i]);
                        }
                        collidemap[j] = true;
                    }
                }
                else if(collidemap[j])
                {
                    if(m_colliders[i]->isTrigger() || m_colliders[j]->isTrigger())
                    {
                        m_colliders[i]->GetGameObject()->OnTriggerExit(m_colliders[j]);
                        m_colliders[j]->GetGameObject()->OnTriggerExit(m_colliders[i]);
                    }
                    else 
                    {
                        m_colliders[i]->GetGameObject()->OnColliderExit(m_colliders[j]);
                        m_colliders[j]->GetGameObject()->OnColliderExit(m_colliders[i]);
                    }
                    collidemap[j] = false;
                }
            }
            collidemap += m_maxcollidercount;
        }
    }

    void Physics::CheckDeleted()
    {
        byte *itercollidemap = nullptr;
        byte *selfcollidemap = m_collidemap;
        for(Uint32 i=0; i<m_maxcollidercount; ++i)
        {
            if(m_colliders[i] && m_colliders[i]->GetGameObject()->isDeleted())
            {
                m_colliders[i] = nullptr;
                itercollidemap = m_collidemap;
                for(Uint32 j=0; j<i; ++j)
                {   
                    if(itercollidemap[i])
                    {
                        m_colliders[i]->GetGameObject()->OnColliderExit(m_colliders[j]);
                        m_colliders[j]->GetGameObject()->OnColliderExit(m_colliders[i]);
                        itercollidemap[i] = 0;
                    }
                    itercollidemap += m_maxcollidercount;
                }
                for(Uint32 j=i+1; j<m_maxcollidercount; ++j)
                {
                    if(selfcollidemap[j])
                    {
                        m_colliders[i]->GetGameObject()->OnColliderExit(m_colliders[j]);
                        m_colliders[j]->GetGameObject()->OnColliderExit(m_colliders[i]);
                        selfcollidemap[j] = 0;
                    }
                }

                --m_collidercount;
            }
            selfcollidemap += m_maxcollidercount;
        }
    }

    Collider *Physics::Register(Collider *collider)
    {
        for(Uint32 i=0; i<m_maxcollidercount; ++i)
        {
            if(!m_colliders[i])
            {
                m_colliders[i] = collider;
                ++m_collidercount;
                return collider;
            }
        }
        return nullptr;
    }

    float32 Physics::GetGravity()const
    {
        return m_gravity;
    }

    void Physics::SetGravity(float32 a)
    {
        m_gravity = a;
    }

    Uint32 Physics::GetColliderCount()const
    {
        return m_collidercount;
    }

    Uint32 Physics::GetMaxColliderCount()const
    {
        return m_maxcollidercount;
    }

    
    // Rigidbody class
    Rigidbody::Rigidbody()
    :Component(), m_mass(0.0f), m_drag(0.0f), m_velocity()
    {
        SetSimulated(true);
    }

    Rigidbody::Rigidbody(const Rigidbody &other)
    :Component(other), m_mass(other.m_mass), m_drag(other.m_drag), m_velocity(other.m_velocity)
    {
        this->SetSimulated(other.isSimulated());
    }

    Rigidbody::~Rigidbody()
    {
        ;
    }

    Rigidbody &Rigidbody::operator=(const Rigidbody &other)
    {
        assert(this != &other);
        Component::operator=(other);
        this->m_mass = other.m_mass;
        this->m_drag = other.m_drag;
        this->m_velocity = other.m_velocity;
        this->SetSimulated(other.isSimulated());
        return (*this);
    }

    void Rigidbody::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        Component::InitWithJson(obj, allocator);

        assert(obj.HasMember("mass"));
        assert(obj.HasMember("drag"));
        assert(obj.HasMember("isSimulated"));
        assert(obj.HasMember("isGravityActive"));
        assert(obj["mass"].IsFloat());
        assert(obj["drag"].IsFloat());
        assert(obj["isSimulated"].IsBool());
        assert(obj["isGravityActive"].IsBool());

        m_mass = obj["mass"].GetFloat();
        m_drag = obj["drag"].GetFloat();
        SetSimulated(obj["isSimulated"].GetBool());
        SetGravityActive(obj["isGravityActive"].GetBool());
    }

    void Rigidbody::Start()
    {
        Component::Start();
    }

    void Rigidbody::Update()
    {
        if(isSimulated())
        {
            Time &t = Time::Get();
            AddForce(m_velocity * (float32)(-m_drag*t.GetDeltatime()/m_mass));
            glm::vec3 delta =  m_velocity * (float32)t.GetDeltatime();
            Move(delta);
        }
    }
    void Rigidbody::Release()
    {
        ;
    }

    void Rigidbody::OnColliderEnter(Collider *other)
    {
        Rigidbody *otherrigid = other->GetRigidbody();

        const glm::vec3 &v1 = this->GetVelocity();
        const glm::vec3 &v2 = otherrigid->GetVelocity();
        float32 m1 = this->GetMass();
        float32 m2 = otherrigid->GetMass();
        
        glm::vec3 u1(0.0f);
        u1 = ((m1-m2)*v1 + 2*m2*v2)/(m1+m2);

        this->SetVelocity(u1);
    }

    void Rigidbody::Accelerate(const glm::vec3 &a)
    {
        m_velocity += a;
    }

    void Rigidbody::Accelerate(float32 x, float32 y, float32 z)
    {
        m_velocity.x += x;
        m_velocity.y += y;
        m_velocity.z += z;
    }

    void Rigidbody::AddForce(const glm::vec3 &f)
    {
        m_velocity += f / m_mass;
    }

    void Rigidbody::AddForce(float32 x, float32 y, float32 z)
    {
        m_velocity.x += x / m_mass;
        m_velocity.y += y / m_mass;
        m_velocity.z += z / m_mass;
    }

    float32 Rigidbody::GetMass()const
    {
        return m_mass;
    }

    const glm::vec3 &Rigidbody::GetVelocity()const
    {
        return m_velocity;
    }

    int32 Rigidbody::isSimulated()const
    {
        return m_flags.GetFlag(2);
    }

    int32 Rigidbody::isGravityActive()const
    {
        return m_flags.GetFlag(3);
    }

    void Rigidbody::SetMass(float32 m)
    {
        m_mass = m;
    }

    void Rigidbody::SetVelocity(const glm::vec3 &v)
    {
        m_velocity = v;
    }

    void Rigidbody::SetVelocity(float32 x, float32 y, float32 z)
    {
        m_velocity.x = x;
        m_velocity.y = y;
        m_velocity.z = z;
    }

    void Rigidbody::SetSimulated(int32 val)
    {
        m_flags.SetFlag(2, val);
    }

    void Rigidbody::SetGravityActive(int32 val)
    {
        m_flags.SetFlag(3, val);
    }
}