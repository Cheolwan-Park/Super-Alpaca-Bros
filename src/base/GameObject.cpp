#include "GameObject.hpp"
#include "Application.hpp"
#include "Scene.hpp"

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

    GameObject::GameObject(const rapidjson::GenericObject<true, rapidjson::Value> &obj)
    :m_id(0), m_position(0.0f, 0.0f, 0.0f), m_parent(nullptr), m_flags()
    {
        assert(obj.HasMember("GameObject"));
        assert(obj["GameObject"].IsObject());
        auto gobj = obj["GameObject"].GetObject();

        assert(gobj.HasMember("id"));
        assert(gobj.HasMember("pos"));
        assert(gobj["id"].IsString());
        assert(gobj["pos"].IsObject());

        const char *id = gobj["id"].GetString();
        m_id = CompileTimeHash::runtime_hash(id, strlen(id));
        JsonParseMethods::ReadVector(gobj["pos"].GetObject(), &m_position);

        if(gobj.HasMember("parent"))
        {
            Application &app = Application::Get();
            Scene *scene = app.GetScene();
            assert(scene);
            if(gobj["parent"].IsObject())
            {
                auto parentdata = gobj["parent"].GetObject();
                assert(parentdata.HasMember("storage"));
                assert(parentdata.HasMember("id"));
                assert(parentdata["storage"].IsString());
                assert(parentdata["id"].IsString());
                const char *storage_id = parentdata["storage"].GetString();
                const char *parent_id = parentdata["id"].GetString();
                m_parent = scene->GetObject(CompileTimeHash::runtime_hash(storage_id, strlen(storage_id)),
                                            CompileTimeHash::runtime_hash(parent_id, strlen(parent_id)));
            }
            else if(gobj["parent"].IsString())
            {
                const char *parent_id = gobj["parent"].GetString();
                m_parent = scene->GetObject(CompileTimeHash::runtime_hash(parent_id, strlen(parent_id)));
            }
        }
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
    :Storage<GameObject>(), m_id(id), m_order(order)
    {
        SetFreeFunc(FreeObject);
    }
    
    ObjectStorage::~ObjectStorage()
    {
        Clear();
    }
    
    void ObjectStorage::UpdateObjects()
    {
        ForDo(UpdateObject);
    }
    
    void ObjectStorage::CheckAndDeleteObjects()
    {
        ForDo(CheckAndDeleteObject);
    }
    
    Uint32 ObjectStorage::GetID()const
    {
        return m_id;
    }
    
    Uint32 ObjectStorage::GetOrder()const
    {
        return m_order;
    }

    void ObjectStorage::UpdateObject(GameObject **gobj)
    {
        if((*gobj)->isAvailable())
            (*gobj)->Update();
    }

    void ObjectStorage::CheckAndDeleteObject(GameObject **gobj)
    {
        if((*gobj)->isDeleted())
        {
            FreeObject(gobj);
        }
    }

    void ObjectStorage::FreeObject(GameObject **gobj)
    {
        (*gobj)->~GameObject();
        (*gobj) = nullptr;
    }
}











