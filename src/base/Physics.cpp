#include "Physics.hpp"
#include "Collider.hpp"

namespace Base
{
    Physics::Physics()
    :m_collidercount(0), m_maxcollidercount(0), m_colliders(nullptr), m_collidemap(nullptr)
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
        assert(obj["collider_count"].IsInt());

        m_maxcollidercount = obj["collider_count"].GetInt();
        
        m_colliders = allocator.Alloc<Collider*>(m_maxcollidercount);
        m_collidemap = (byte*)allocator.Alloc<byte>(m_maxcollidercount*m_maxcollidercount);
    }

    void Physics::Update()
    {
        byte *collidemap = nullptr;
        for(Uint32 i=0; i<m_maxcollidercount; ++i)
        {
            if(!m_colliders[i])
            {
                continue;
            }
            for(Uint32 j=i+1; j<m_maxcollidercount; ++j)
            {
                if(!m_colliders[j])
                {
                    continue;
                }
                if(m_colliders[i]->isCollideWith(m_colliders[j]))
                {
                    if(collidemap[j])
                    {
                        m_colliders[i]->GetGameObject()->OnColliderStay(m_colliders[j]);
                        m_colliders[j]->GetGameObject()->OnColliderStay(m_colliders[i]);
                    }
                    else
                    {
                        m_colliders[i]->GetGameObject()->OnColliderEnter(m_colliders[j]);
                        m_colliders[j]->GetGameObject()->OnColliderEnter(m_colliders[i]);
                        collidemap[j] = 1;
                    }
                }
                else if(collidemap[j])
                {
                    m_colliders[i]->GetGameObject()->OnColliderExit(m_colliders[j]);
                    m_colliders[j]->GetGameObject()->OnColliderExit(m_colliders[i]);
                    collidemap[j] = 0;
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

    Uint32 Physics::GetColliderCount()const
    {
        return m_collidercount;
    }

    Uint32 Physics::GetMaxColliderCount()const
    {
        return m_maxcollidercount;
    }

    Physics &Physics::GetGlobal()
    {
        static Physics instance;
        return instance;
    }
}