#include "GameObject.hpp"

namespace Base
{
    // GameObject class
    GameObject::GameObject(Uint32 id, int32 isStatic)
    :m_id(id), m_position(0.0f, 0.0f, 0.0f), m_parent(nullptr), m_flags()
    {
        SetAvailable(true);
        m_flags.SetFlag(3, isStatic);
    }
    
    GameObject::GameObject(Uint32 id, const GameObject *parent, int32 isStatic)
    :m_id(id), m_position(0.0f, 0.0f, 0.0f), m_parent(parent), m_flags()
    {
        SetAvailable(true);
        m_flags.SetFlag(3, isStatic);
    }
    
    GameObject::GameObject(const GameObject &other)
    :m_id(0), m_position(other.m_position), m_parent(other.m_parent), m_flags()
    {
        SetAvailable(true);
    }
    
    GameObject::~GameObject()
    {
        ;
    }
    
    GameObject &GameObject::operator=(const GameObject &other)
    {
        assert(this != &other);
        this->m_position = other.m_position;
        this->m_parent = other.m_parent;
        return (*this);
    }
    
    void GameObject::Start()
    {
        m_flags.SetFlag(2, true);
    }

    void GameObject::Update()
    {
        ;
    }
    
    GameObject *GameObject::AddChild(GameObject *child)const
    {
        child->SetParent(this);
        return child;
    }
    
    Uint32 GameObject::GetID()const
    {
        return m_id;
    }
    
    const glm::vec3 &GameObject::GetLocalPosition()const
    {
        return m_position;
    }
    
    const glm::vec3 GameObject::GetWorldPosition()const
    {
        if(nullptr == m_parent)
            return m_position;
        else
        {
            const GameObject *parent = m_parent;
            glm::vec3 result = m_position + parent->GetLocalPosition();
            while(nullptr != (parent = parent->GetParent()))
            {
                result += parent->GetLocalPosition();
            }
            return result;
        }
    }
    
    const GameObject *GameObject::GetParent()const
    {
        return m_parent;
    }
    
    int32 GameObject::isDeleted()const
    {
        return m_flags.GetFlag(0);
    }
    
    int32 GameObject::isAvailable()const
    {
        return m_flags.GetFlag(1);
    }
    
    int32 GameObject::isStarted()const
    {
        return m_flags.GetFlag(2);
    }
    
    int32 GameObject::isStatic()const
    {
        return m_flags.GetFlag(3);
    }
    
    void GameObject::SetID(Uint32 id)
    {
        if(false == m_flags.GetFlag(4)) {
            m_id = id;
            m_flags.SetFlag(4, true);
        }
    }
    
    void GameObject::SetLocalPosition(const glm::vec3 &position)
    {
        m_position = position;
    }
    
    void GameObject::SetLocalPosition(float32 x, float32 y)
    {
        m_position.x = x;
        m_position.y = y;
    }
    
    void GameObject::SetLocalPosition(float32 x, float32 y, float32 z)
    {
        m_position.x = x;
        m_position.y = y;
        m_position.z = z;
    }
    
    void GameObject::SetWorldPosition(const glm::vec3 &position)
    {
        m_position = position - m_parent->GetWorldPosition();
    }
    
    void GameObject::SetWorldPosition(float32 x, float32 y)
    {
        glm::vec3 pos(x,y,m_position.z);
        m_position = pos - m_parent->GetWorldPosition();
    }
    
    void GameObject::SetWorldPosition(float32 x, float32 y, float32 z)
    {
        glm::vec3 pos(x,y,z);
        m_position = pos - m_parent->GetWorldPosition();
    }
    
    void GameObject::Move(const glm::vec3 &delta)
    {
        m_position += delta;
    }
    
    void GameObject::Move(float32 x, float32 y)
    {
        m_position.x += x;
        m_position.y += y;
    }
    
    void GameObject::SetParent(const GameObject *parent)
    {
        m_parent = parent;
    }
    
    void GameObject::Delete()
    {
        m_flags.SetFlag(0, true);
    }
    
    void GameObject::SetAvailable(int32 value)
    {
        m_flags.SetFlag(1, value);
    }
    
    
    // Object Storage
    ObjectStorage::ObjectStorage(Uint32 id, Uint32 order)
    :m_id(id), m_order(order), m_objects(nullptr), m_objects_len(0)
    {
        ;
    }
    
    ObjectStorage::~ObjectStorage()
    {
        if(nullptr != m_objects)
        {
            for(Uint32 i=0; i<m_objects_len; ++i)
            {
                if(nullptr != m_objects[i])
                {
                    m_objects[i]->~GameObject();
                    m_objects[i] = nullptr;
                }
            }
        }
    }
    
    void ObjectStorage::AssignMemory(void *memory, Uint32 len)
    {
        m_objects = (GameObject**)memory;
        m_objects_len = len;
    }
    
    int32 ObjectStorage::Register(ObjectStorage::Type object)
    {
        assert(m_objects);
        assert(object);
        
        Uint32 idx = (object->GetID())%m_objects_len;
        if(nullptr != m_objects[idx])
            return RET_FAILED;
        
        m_objects[idx] = object;
        m_objects[idx]->Start();
        
        return RET_SUCC;
    }
    
    const ObjectStorage::Type ObjectStorage::operator[](Uint32 id)const
    {
        Uint32 idx = id%m_objects_len;
        return m_objects[idx];
    }
    
    ObjectStorage::Type ObjectStorage::operator[](Uint32 id)
    {
        Uint32 idx = id%m_objects_len;
        return m_objects[idx];
    }
    
    void ObjectStorage::UpdateObjects()
    {
        if(nullptr != m_objects)
        {
            for(Uint32 i=0; i<m_objects_len; ++i)
            {
                if(nullptr != m_objects[i] && m_objects[i]->isAvailable())
                    m_objects[i]->Update();
            }
        }
    }
    
    void ObjectStorage::CheckAndDeleteObjects()
    {
        if(nullptr != m_objects)
        {
            for(Uint32 i=0; i<m_objects_len; ++i)
            {
                if(nullptr != m_objects[i] && m_objects[i]->isDeleted())
                {
                    m_objects[i]->~GameObject();
                    m_objects[i] = nullptr;
                }
            }
        }
    }
    
    Uint32 ObjectStorage::GetID()const
    {
        return m_id;
    }
    
    Uint32 ObjectStorage::GetOrder()const
    {
        return m_order;
    }
}











