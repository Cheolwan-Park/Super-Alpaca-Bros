#include "Ground.hpp"

namespace Game
{
    Ground::Ground()
    :BoxCollider()
    {
        ;
    }

    Ground::Ground(const Ground &other)
    :BoxCollider(other)
    {
        ;
    }

    Ground::~Ground()
    {
        ;
    }

    Ground &Ground::operator=(const Ground &other)
    {
        assert(this != &other);
        BoxCollider::operator=(other);
        return (*this);
    }

    void Ground::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        BoxCollider::InitWithJson(obj, allocator);

        assert(obj.HasMember("ignorelist_size"));
        assert(obj.HasMember("isPassable"));
        assert(obj["isPassable"].IsBool());
        assert(obj["ignorelist_size"].IsInt());

        m_ignorelistsize = obj["ignorelist_size"].GetInt();
        m_ignorelist = allocator.Alloc<Collider*>(m_ignorelistsize);
        memset(m_ignorelist, 0, sizeof(Collider*)*m_ignorelistsize);
        SetPassable(obj["isPassable"].GetBool());
    }

    void Ground::OnTriggerStay(Collider *other)
    {
        if(other->GetTag() != "player"_hash)
        {
            return;
        }

        for(Uint32 i=0; i<m_ignorelistsize; ++i)
        {
            if(other == m_ignorelist[i])
                return;
        }

        glm::vec3 otherpos(0.0f), thispos(0.0f);
        other->GetWorldPosition(&otherpos);
        this->GetWorldPosition(&thispos);

        float32 distance = GetBox().h/2.0f;
        switch(other->GetType())
        {
            case Collider::Type::BOX:
            {
                distance += ((BoxCollider*)other)->GetBox().h/2.0f;
                break;
            }
            case Collider::Type::CIRCLE:
            {
                distance += ((CircleCollider*)other)->GetRadius();
                break;
            }
            default:
            {
                fprintf(stderr, "invalid collider type\n");
                exit(-1);
                break;
            }
        }

        if(otherpos.y > thispos.y && otherpos.y < (thispos.y + distance))
        {
            if(other->GetTag() != "player"_hash)
            {
                return;
            }

            Rigidbody *otherrigid = other->GetRigidbody();
            if(otherrigid)
            {
                const glm::vec3 &otherv = otherrigid->GetVelocity();
                if(otherv.y >= 0.0f)
                {
                    return;
                }
                otherrigid->SetVelocity(otherv.x, 0.0f, otherv.z);
            }

            otherpos.y = thispos.y + distance - 0.05f;
            other->SetWorldPosition(otherpos);
        }
    }

    void Ground::OnTriggerExit(Collider *other)
    {
        for(Uint32 i=0; i<m_ignorelistsize; ++i)
        {
            if(m_ignorelist[i] == other)
            {
                m_ignorelist[i] = nullptr;
                break;
            }
        }
    }

    void Ground::Pass(GameObject *obj)
    {
        assert(obj);

        if(isPassable())
        {
            Collider *collider = obj->GetComponent<BoxCollider>();
            if(!collider)
                collider = obj->GetComponent<CircleCollider>();
            if(!collider)
                return;
            for(Uint32 i=0; i<m_ignorelistsize; ++i)
            {
                if(!m_ignorelist[i])
                {
                    m_ignorelist[i] = collider;
                    break;
                }
            }
        }
    }

    int32 Ground::isPassable()const
    {
        return m_flags.GetFlag(3);
    }

    void Ground::SetPassable(int32 val)
    {
        m_flags.SetFlag(3, val);
    }
}