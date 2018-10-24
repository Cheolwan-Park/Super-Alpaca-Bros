#include "Component.hpp"

namespace Base
{
    // Component class
    Component *Component::Factory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator)
    {
        fprintf(stderr, "Component is abstract class\n");
        return nullptr;
    }

    Component::Component()
    :m_gameobject(nullptr), m_flags()
    {
        ;
    }

    Component::Component(const Component &other)
    :m_gameobject(nullptr), m_flags()
    {
        m_flags.SetFlag(0, other.m_flags.GetFlag(0));
    }

    Component::~Component()
    {
        ;
    }

    Component &Component::operator=(const Component &other)
    {
        assert(this != &other);
        this->m_flags.SetFlag(0, other.m_flags.GetFlag(0));
    }

    void Component::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        assert(obj.HasMember("available"));
        assert(obj["available"].IsBool());

        m_flags.SetFlag(0, obj["available"].GetBool());
    }

    void Component::Start()
    {
        m_flags.SetFlag(1, true);
    }

    ComponentObject *Component::GetGameObject()
    {
        return m_gameobject;
    }

    const ComponentObject *Component::GetGameObject()const
    {
        return m_gameobject;
    }

    int32 Component::isAvailable()const
    {
        return m_flags.GetFlag(0);
    }

    int32 Component::isStarted()const
    {
        return m_flags.GetFlag(1);
    }

    void Component::SetGameObject(ComponentObject *gameobject)
    {
        m_gameobject = gameobject;
    }

    void Component::SetAvailable(int32 val)
    {
        m_flags.SetFlag(0, val);
    }

    
    // ComponentObject class

    GameObject *ComponentObject::Factory(const rapidjson::Value::Object &obj, StackAllocator &allocator, ObjectStorage *storage, Uint32 id)
    {
        assert(storage);
        GameObject *result = new (allocator.Alloc<ComponentObject>()) ComponentObject();
        assert(result);
        result->SetID(id);
        storage->Register(result, result->GetID());
        result->InitWithJson(obj, allocator);
        return result;
    }

    ComponentObject::ComponentObject()
    :GameObject(), m_componentcount(0), m_components(nullptr)
    {
        ;
    }

    ComponentObject::ComponentObject(Uint32 id, int32 isStatic)
    :GameObject(id, isStatic), m_componentcount(0), m_components(nullptr)
    {
        ;
    }

    ComponentObject::ComponentObject(Uint32 id, const GameObject *parent, int32 isStatic)
    :GameObject(id, parent, isStatic), m_componentcount(0), m_components(nullptr)
    {
        ;
    }

    ComponentObject::ComponentObject(const ComponentObject &other)
    :GameObject(other), m_componentcount(0), m_components(nullptr)
    {
        ;
    }

    ComponentObject::~ComponentObject()
    {
        ;
    }

    ComponentObject &ComponentObject::operator=(const ComponentObject &other)
    {
        assert(this != &other);
        GameObject::operator=(other);
        return (*this);
    }

    void ComponentObject::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        GameObject::InitWithJson(obj, allocator);

        assert(obj.HasMember("components"));
        assert(obj["components"].IsArray());

        auto compolist = obj["components"].GetArray();
        m_componentcount = compolist.Size();

        auto &factory = ComponentFactory::GetGlobal();
        m_components = allocator.Alloc<Component*>(m_componentcount);
        Component::FactoryFunc *factoryfunc = nullptr;
        Component *newcompo = nullptr;
        const char *type = nullptr;

        for(int i=0; i<m_componentcount; ++i)
        {
            assert(compolist[i].IsObject());
            auto componentobj = compolist[i].GetObject();

            assert(componentobj.HasMember("type"));
            assert(componentobj["type"].IsString());

            type = componentobj["type"].GetString();
            factoryfunc = factory.GetFunction(CompileTimeHash::runtime_hash(type, strlen(type)));
            assert(factoryfunc);
            newcompo = factoryfunc(componentobj, allocator);
            assert(newcompo);

            newcompo->SetGameObject(this);
            m_components[i] = newcompo;
        }
    }

    void ComponentObject::Start()
    {
        for(int i=0; i<m_componentcount; ++i)
        {
            m_components[i]->Start();
        }
        GameObject::Start();
    }

    void ComponentObject::Update()
    {
        GameObject::Update();
        for(int i=0; i<m_componentcount; ++i)
        {
            m_components[i]->Update();
        }
    }

    void ComponentObject::Release()
    {
        for(int i=0; i<m_componentcount; ++i)
        {
            m_components[i]->Release();
            m_components[i]->~Component();
            m_components[i] = nullptr;
        }
        GameObject::Release();
    }

    Uint32 ComponentObject::GetComponentCount()const 
    {
        return m_componentcount;
    }
    
}