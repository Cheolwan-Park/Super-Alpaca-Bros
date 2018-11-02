#include "GameObject.hpp"
#include "Application.hpp"
#include "Scene.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Base
{
    // GameObject class
    GameObject *GameObject::Factory(const rapidjson::Value::Object &obj, StackAllocator &allocator, 
                                    ObjectStorage *storage, Uint32 id)
    {
        assert(storage);
        GameObject *result = new (allocator.Alloc<GameObject>()) GameObject();
        assert(result);
        result->SetID(id);
        storage->Register(result);
        result->InitWithJson(obj, allocator);
        return result;
    }

    GameObject::GameObject()
    :m_id(0), m_componentcount(0), m_rotation(0.0f), m_components(nullptr),
    m_parent(nullptr), m_position(), m_scale(), m_model(), m_flags()
    {
        SetAvailable(true);
    }

    GameObject::GameObject(Uint32 id, int32 isStatic)
    :m_id(id), m_componentcount(0), m_rotation(0.0f), m_components(nullptr),
    m_parent(nullptr), m_position(), m_scale(), m_model(), m_flags()
    {
        SetAvailable(true);
        m_flags.SetFlag(3, isStatic);
    }
    
    GameObject::GameObject(Uint32 id, const GameObject *parent, int32 isStatic)
    :m_id(id), m_componentcount(0), m_rotation(0.0f), m_components(nullptr),
    m_parent(parent), m_position(), m_scale(), m_model(), m_flags()
    {
        SetAvailable(true);
        m_flags.SetFlag(3, isStatic);
    }
    
    GameObject::GameObject(const GameObject &other)
    :m_id(0), m_componentcount(0), m_rotation(other.m_rotation), m_components(nullptr),
    m_parent(other.m_parent), m_position(other.m_position), m_scale(other.m_scale), 
    m_model(), m_flags()
    {
        m_flags.SetFlag(1, other.m_flags.GetFlag(1));
        m_flags.SetFlag(3, other.m_flags.GetFlag(3));
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
        this->m_scale = other.m_scale;
        this->m_rotation = other.m_rotation;
        this->m_flags.SetFlag(1, other.m_flags.GetFlag(1));
        this->m_flags.SetFlag(3, other.m_flags.GetFlag(3));
        return (*this);
    }

    void GameObject::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        assert(obj.HasMember("tag"));
        assert(obj.HasMember("available"));
        assert(obj.HasMember("static"));
        assert(obj.HasMember("position"));
        assert(obj.HasMember("scale"));
        assert(obj.HasMember("rotation"));
        assert(obj.HasMember("components"));
        assert(obj["tag"].IsString());
        assert(obj["available"].IsBool());
        assert(obj["static"].IsBool());
        assert(obj["position"].IsObject());
        assert(obj["scale"].IsObject());
        assert(obj["rotation"].IsFloat());
        assert(obj["components"].IsArray());

        // read elements
        const char *tag = obj["tag"].GetString();
        m_tag = CompileTimeHash::runtime_hash(tag, strlen(tag));
        SetAvailable(obj["available"].GetBool());
        m_flags.SetFlag(3, obj["static"].GetBool());
        JsonParseMethods::ReadVector(obj["position"].GetObject(), &m_position);
        JsonParseMethods::ReadVector2(obj["scale"].GetObject(), &m_scale);
        m_rotation = obj["rotation"].GetFloat();

        // read components
        auto compolist = obj["components"].GetArray();
        m_componentcount = compolist.Size();

        auto &factory = ComponentFactory::GetGlobal();
        m_components = allocator.Alloc<Component*>(m_componentcount);
        Component::FactoryFunc *factoryfunc = nullptr;
        Component *newcompo = nullptr;
        const char *type = nullptr;
        
        // read parent
        if(obj.HasMember("parent"))
        {
            assert(obj["parent"].IsObject());
            Application &app = Application::Get();
            Scene *scene = app.GetScene();
            assert(scene);
            auto parentdata = obj["parent"].GetObject();
            assert(parentdata.HasMember("storage"));
            assert(parentdata.HasMember("id"));
            assert(parentdata["storage"].IsString());
            assert(parentdata["id"].IsString());

            const char *storageidstr = parentdata["storage"].GetString();
            const char *idstr = parentdata["id"].GetString();
            Uint32 storageid = CompileTimeHash::runtime_hash(storageidstr, strlen(storageidstr));
            Uint32 id = CompileTimeHash::runtime_hash(idstr, strlen(idstr));
            m_parent = scene->GetObject(storageid, id);
            assert(m_parent);
        }

        for(int i=0; i<m_componentcount; ++i)
        {
            assert(compolist[i].IsObject());
            auto componentobj = compolist[i].GetObject();

            assert(componentobj.HasMember("type"));
            assert(componentobj["type"].IsString());

            type = componentobj["type"].GetString();
            factoryfunc = factory.GetFunction(CompileTimeHash::runtime_hash(type, strlen(type)));
            assert(factoryfunc);
            newcompo = factoryfunc(componentobj, allocator, this);
            assert(newcompo);

            newcompo->SetGameObject(this);
            m_components[i] = newcompo;
        }

    }
    
    void GameObject::Start()
    {
        // set model matrix
        m_model = glm::mat3x3(1.0f);
        glm::vec2 localpos = GetLocalPosition();
        m_model = glm::translate(m_model, localpos);
        m_model = glm::rotate(m_model, GetRotation());
        m_model = glm::scale(m_model, GetScale());

        for(int i=0; i<m_componentcount; ++i)
        {
            m_components[i]->Start();
        }

        m_flags.SetFlag(2, true);
    }

    void GameObject::Update()
    {
        if(!isStarted())
            Start();

        if(!isStatic())
        {
            // update model matrix
            m_model = glm::mat3x3(1.0f);
            glm::vec2 localpos = GetLocalPosition();
            m_model = glm::translate(m_model, localpos);
            m_model = glm::rotate(m_model, GetRotation());
            m_model = glm::scale(m_model, GetScale());
        }

        for(int i=0; i<m_componentcount; ++i)
        {
            m_components[i]->Update();
        }
    }

    void GameObject::Release()
    {
        for(int i=0; i<m_componentcount; ++i)
        {
            m_components[i]->Release();
            m_components[i]->~Component();
            m_components[i] = nullptr;
        }
    }
    
    GameObject *GameObject::AddChild(GameObject *child)const
    {
        child->SetParent(this);
        return child;
    }

    void GameObject::OnColliderEnter(const Collider *other)
    {
        for(int i=0; i<m_componentcount; ++i)
        {
            m_components[i]->OnColliderEnter(other);
        }
    }

    void GameObject::OnColliderStay(const Collider *other)
    {
        for(int i=0; i<m_componentcount; ++i)
        {
            m_components[i]->OnColliderStay(other);
        }
    }

    void GameObject::OnColliderExit(const Collider *other)
    {
        for(int i=0; i<m_componentcount; ++i)
        {
            m_components[i]->OnColliderExit(other);
        }
    }
    
    Uint32 GameObject::GetID()const
    {
        return m_id;
    }

    Uint32 GameObject::GetTag()const
    {
        return m_tag;
    }
    
    const glm::vec3 &GameObject::GetLocalPosition()const
    {
        return m_position;
    }
    
    void GameObject::GetWorldPosition(glm::vec3 *vec)const
    {
        if(!m_parent) 
            *vec = m_position;
        else
        {
            glm::mat3x3 model(1.0f);
            GetModel(&model);
            (*vec).x = (*vec).y = 0.0f;
            (*vec).z = 1.0f;
            *vec = model * (*vec);
        }
    }

    const glm::vec2 &GameObject::GetScale()const
    {
        return m_scale;
    }
    
    float32 GameObject::GetRotation()const
    {
        return m_rotation;
    }

    const glm::mat3x3 &GameObject::GetLocalModel()const
    {
        return m_model;
    }

    void GameObject::GetModel(glm::mat3x3 *mat)const
    {
        if(!m_parent)
            *mat = m_model;
        else
        {
            const GameObject *parent = m_parent;
            *mat = (parent->GetLocalModel()) * (this->GetLocalModel());
            while(nullptr != (parent = parent->GetParent()))
            {
                *mat = (parent->GetLocalModel()) * (*mat);
            }
        }
    }

    Uint32 GameObject::GetComponentCount()const 
    {
        return m_componentcount;
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

    void GameObject::SetTag(Uint32 tag)
    {
        m_tag = tag;
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
        if(!m_parent)
            m_position = position;
        else
        {
            glm::mat3x3 model(1.0f);
            m_parent->GetModel(&model);
            model = glm::inverse(model);
            m_position = model * position;
        }
    }
    
    void GameObject::SetWorldPosition(float32 x, float32 y)
    {
        if(!m_parent)
        {
            m_position.x = x;
            m_position.y = y;
        }
        else
        {
            glm::vec3 pos(x, y, m_position.z);
            glm::mat3x3 model(1.0f);
            m_parent->GetModel(&model);
            model = glm::inverse(model);
            m_position = model * pos;
        }
    }
    
    void GameObject::SetWorldPosition(float32 x, float32 y, float32 z)
    {
        if(!m_parent)
        {
            m_position.x = x;
            m_position.y = y;
            m_position.z = z;
        }
        else
        {
            glm::vec3 pos(x, y, z);
            glm::mat3x3 model(1.0f);
            m_parent->GetModel(&model);
            model = glm::inverse(model);
            m_position = model * pos;
        }
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

    void GameObject::SetScale(const glm::vec2 &val)
    {
        m_scale = val;
    }
    
    void GameObject::SetScale(float32 x, float32 y)
    {
        m_scale.x = x;
        m_scale.y = y;
    }
    
    void GameObject::Scale(const glm::vec2 &val)
    {
        m_scale.x *= val.x;
        m_scale.y *= val.y;
    }
    
    void GameObject::Scale(float32 x, float32 y)
    {
        m_scale.x *= x;
        m_scale.y *= y;
    }
    
    void GameObject::Scale(float32 x)
    {
        m_scale *= x;
    }
    
    void GameObject::SetRotation(float32 val)
    {
        m_rotation = val;
    }
    
    void GameObject::Rotate(float32 delta)
    {
        m_rotation += delta;
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
    :m_id(id), m_order(order), m_len(0), m_gameobjects(nullptr)
    {
        ;
    }
    
    ObjectStorage::~ObjectStorage()
    {
        Clear();
    }

    void ObjectStorage::AssignMemory(void *mem, Uint32 len)
    {
        m_len = len;
        m_gameobjects = (GameObject**)mem;
        memset(m_gameobjects, 0, sizeof(Type)*len);
    }

    GameObject *ObjectStorage::Register(GameObject *gameobject)
    {
        for(Uint32 i=0; i<m_len; ++i)
        {
            if(!m_gameobjects[i])
            {
                m_gameobjects[i] = gameobject;
                return gameobject;
            }
        }
        fprintf(stderr, "there is no space in ObjectStorage\n");
        exit(-1);
        return nullptr;
    }

    GameObject *ObjectStorage::Get(Uint32 hash)
    {
        for(Uint32 i=0; i<m_len; ++i)
        {
            if(m_gameobjects[i] && hash == m_gameobjects[i]->GetID())
            {
                return m_gameobjects[i];
            }
        }
        return nullptr;
    }
    
    void ObjectStorage::UpdateObjects()
    {
        for(Uint32 i=0; i<m_len; ++i)
        {
            if(m_gameobjects[i] && m_gameobjects[i]->isAvailable())
            {
                m_gameobjects[i]->Update();
            }
        }
    }
    
    void ObjectStorage::CheckAndDelete()
    {
        for(Uint32 i=0; i<m_len; ++i)
        {
            if(m_gameobjects[i] && m_gameobjects[i]->isDeleted())
            {
                m_gameobjects[i]->Release();
                m_gameobjects[i]->~GameObject();
                m_gameobjects[i] = nullptr;
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

    void ObjectStorage::Clear()
    {
        for(Uint32 i=0; i<m_len; ++i)
        {
            if(m_gameobjects[i])
            {
                m_gameobjects[i]->Release();
                m_gameobjects[i]->~GameObject();
                m_gameobjects[i] = nullptr;
            }
        }
    }
}











