#include "Component.hpp"
#include "GameObject.hpp"

namespace Base
{
    // Component class
    Component* Component::Factory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator, GameObject *gameobject)
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

    void Component::OnColliderEnter(Collider *other)
    {
        ;
    }

    void Component::OnColliderStay(Collider *other)
    {
        ;
    }

    void Component::OnColliderExit(Collider *other)
    {
        ;
    }

    void Component::OnTriggerEnter(Collider *other)
    {
        ;
    }

    void Component::OnTriggerStay(Collider *other)
    {
        ;
    }

    void Component::OnTriggerExit(Collider *other)
    {
        ;
    }

    GameObject *Component::GetGameObject()
    {
        return m_gameobject;
    }

    const GameObject *Component::GetGameObject()const
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

    void Component::SetGameObject(GameObject *gameobject)
    {
        m_gameobject = gameobject;
    }

    void Component::SetAvailable(int32 val)
    {
        m_flags.SetFlag(0, val);
    }

    Uint32 Component::GetTag()const
    {
        assert(m_gameobject);
        return m_gameobject->GetTag();
    }

    const glm::vec3 &Component::GetLocalPosition()const
    {
        assert(m_gameobject);
        return m_gameobject->GetLocalPosition();
    }
        
    void Component::GetWorldPosition(glm::vec3 *vec)const
    {
        assert(m_gameobject);
        m_gameobject->GetWorldPosition(vec);
    }
        
    const GameObject *Component::GetParent()const
    {
        assert(m_gameobject);
        return m_gameobject->GetParent();
    }

    const glm::vec2 &Component::GetScale()const
    {
        assert(m_gameobject);
        return m_gameobject->GetScale();
    }
        
    float32 Component::GetRotation()const
    {
        assert(m_gameobject);
        return m_gameobject->GetRotation();
    }

    const glm::mat3x3 &Component::GetLocalModel()const
    {
        assert(m_gameobject);
        return m_gameobject->GetLocalModel();
    }

    void Component::GetModel(glm::mat3x3 *mat)const
    {
        assert(m_gameobject);
        m_gameobject->GetModel(mat);
    }

    void Component::SetTag(Uint32 tag)
    {
        assert(m_gameobject);
        m_gameobject->SetTag(tag);
    }
        
    void Component::SetLocalPosition(const glm::vec3 &position)
    {
        assert(m_gameobject);
        m_gameobject->SetLocalPosition(position);
    }
        
    void Component::SetLocalPosition(float32 x, float32 y)
    {
        assert(m_gameobject);
        m_gameobject->SetLocalPosition(x, y);
    }
        
    void Component::SetLocalPosition(float32 x, float32 y, float32 z)
    {
        assert(m_gameobject);
        m_gameobject->SetLocalPosition(x, y, z);
    }
        
    void Component::SetWorldPosition(const glm::vec3 &position)
    {
        assert(m_gameobject);
        m_gameobject->SetWorldPosition(position);
    }
        
    void Component::SetWorldPosition(float32 x, float32 y)
    {
        assert(m_gameobject);
        m_gameobject->SetWorldPosition(x, y);
    }
        
    void Component::SetWorldPosition(float32 x, float32 y, float32 z)
    {
        assert(m_gameobject);
        m_gameobject->SetWorldPosition(x, y, z);
    }
        
    void Component::Move(const glm::vec3 &delta)
    {
        assert(m_gameobject);
        m_gameobject->Move(delta);
    }
        
    void Component::Move(float32 x, float32 y)
    {
        assert(m_gameobject);
        m_gameobject->Move(x, y);
    }

    void Component::SetScale(const glm::vec2 &val)
    {
        assert(m_gameobject);
        m_gameobject->SetScale(val);
    }
        
    void Component::SetScale(float32 x, float32 y)
    {
        assert(m_gameobject);
        m_gameobject->SetScale(x, y);
    }
        
    void Component::Scale(const glm::vec2 &val)
    {
        assert(m_gameobject);
        m_gameobject->Scale(val);
    }
        
    void Component::Scale(float32 x, float32 y)
    {
        assert(m_gameobject);
        m_gameobject->Scale(x, y);
    }
        
    void Component::Scale(float32 x)
    {
        assert(m_gameobject);
        m_gameobject->Scale(x);
    }
        
    void Component::SetRotation(float32 val)
    {
        assert(m_gameobject);
        m_gameobject->SetRotation(val);
    }
        
    void Component::Rotate(float32 delta)
    {
        assert(m_gameobject);
        m_gameobject->Rotate(delta);
    }
        
    void Component::SetParent(const GameObject *parent)
    {
        assert(m_gameobject);
        m_gameobject->SetParent(parent);
    }
}