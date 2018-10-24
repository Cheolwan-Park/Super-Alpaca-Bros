#include "GameObject.hpp"
#include "Application.hpp"
#include "JsonScene.hpp"
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
        storage->Register(result, result->GetID());
        result->InitWithJson(obj, allocator);
        return result;
    }

    GameObject::GameObject()
    :m_id(0), m_position(), m_parent(nullptr),
    m_scale(), m_rotation(0.0f), m_model(), m_flags()
    {
        SetAvailable(true);
    }

    GameObject::GameObject(Uint32 id, int32 isStatic)
    :m_id(id), m_position(), m_parent(nullptr), 
    m_scale(), m_rotation(0.0f), m_model(), m_flags()
    {
        SetAvailable(true);
        m_flags.SetFlag(3, isStatic);
    }
    
    GameObject::GameObject(Uint32 id, const GameObject *parent, int32 isStatic)
    :m_id(id), m_position(), m_parent(parent), 
    m_scale(), m_rotation(0.0f), m_model(), m_flags()
    {
        SetAvailable(true);
        m_flags.SetFlag(3, isStatic);
    }
    
    GameObject::GameObject(const GameObject &other)
    :m_id(0), m_position(other.m_position), m_parent(other.m_parent), 
    m_scale(other.m_scale), m_rotation(other.m_rotation), m_model(),
    m_flags()
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
        assert(obj.HasMember("available"));
        assert(obj.HasMember("static"));
        assert(obj.HasMember("position"));
        assert(obj.HasMember("scale"));
        assert(obj.HasMember("rotation"));
        assert(obj["available"].IsBool());
        assert(obj["static"].IsBool());
        assert(obj["position"].IsObject());
        assert(obj["scale"].IsObject());
        assert(obj["rotation"].IsFloat());

        SetAvailable(obj["available"].GetBool());
        m_flags.SetFlag(3, obj["static"].GetBool());
        JsonParseMethods::ReadVector(obj["position"].GetObject(), &m_position);
        JsonParseMethods::ReadVector2(obj["scale"].GetObject(), &m_scale);
        m_rotation = obj["rotation"].GetFloat();

        if(obj.HasMember("parent"))
        {
            Application &app = Application::Get();
            JsonScene *scene = static_cast<JsonScene*>(app.GetScene());
            assert(scene);
            if(obj["parent"].IsObject())
            {
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
            else if(obj["parent"].IsString())
            {
                const char *parent_id = obj["parent"].GetString();
                m_parent = scene->GetObject(CompileTimeHash::runtime_hash(parent_id, strlen(parent_id)));
            }
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
    }

    void GameObject::Release()
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
            glm::vec3 worldpos;
            m_parent->GetWorldPosition(&worldpos);
            m_position = pos - worldpos;
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
            glm::vec3 worldpos;
            m_parent->GetWorldPosition(&worldpos);
            m_position = pos - worldpos;
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
        (*gobj)->Release();
        (*gobj)->~GameObject();
        (*gobj) = nullptr;
    }
}











