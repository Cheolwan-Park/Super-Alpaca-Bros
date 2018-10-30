#ifndef BaseComponent_hpp
#define BaseComponent_hpp

#include "Json.hpp"
#include "BitFlag.hpp"
#include "Storage.hpp"
#include "Allocator.hpp"
#include "String.hpp"
#include <glm/glm.hpp>

#ifndef MAKE_TYPE_ID
#define MAKE_TYPE_ID(__TYPENAME__)  \
static constexpr Uint32 ID = ::Base::CompileTimeHash::fnv1a_32(#__TYPENAME__, sizeof(#__TYPENAME__)-1);
#endif

#ifndef COMPONENT
#define COMPONENT(__TYPENAME__)     \
MAKE_TYPE_ID(__TYPENAME__);         \
virtual Uint32 GetTypeID()const  { return ID; }
#endif

namespace Base
{
    class GameObject;
    class Collider;

    class Component 
    {
    public:
        typedef Component* (FactoryFunc)(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator, GameObject *gameobject);

        static Component* Factory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator, GameObject *gameobject);

    public:
        COMPONENT(Component);

        Component();

        // not copying gameobject, isStarted 
        Component(const Component &other);

        virtual ~Component();

        // not copying gameobject, isStarted 
        Component &operator=(const Component &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual void Start();

        virtual void Update() = 0;

        virtual void Release() = 0;

        // collide callback
        virtual void OnColliderEnter(const Collider *other);
        virtual void OnColliderStay(const Collider *other);
        virtual void OnColliderExit(const Collider *other);

        // get
        GameObject *GetGameObject();

        const GameObject *GetGameObject()const;

        int32 isAvailable()const;

        int32 isStarted()const;

        // set 
        void SetGameObject(GameObject *gameobject);

        void SetAvailable(int32 val);

        // gameobject functions wrapping
        Uint32 GetTag()const;

        const glm::vec3 &GetLocalPosition()const;
        
        void GetWorldPosition(glm::vec3 *vec)const;
        
        const GameObject *GetParent()const;

        const glm::vec2 &GetScale()const;
        
        float32 GetRotation()const;

        const glm::mat3x3 &GetLocalModel()const;

        void GetModel(glm::mat3x3 *mat)const;

        void SetTag(Uint32 tag);
        
        void SetLocalPosition(const glm::vec3 &position);
        
        void SetLocalPosition(float32 x, float32 y);
        
        void SetLocalPosition(float32 x, float32 y, float32 z);
        
        void SetWorldPosition(const glm::vec3 &position);
        
        void SetWorldPosition(float32 x, float32 y);
        
        void SetWorldPosition(float32 x, float32 y, float32 z);
        
        void Move(const glm::vec3 &delta);
        
        void Move(float32 x, float32 y);

        void SetScale(const glm::vec2 &val);
        
        void SetScale(float32 x, float32 y);
        
        void Scale(const glm::vec2 &val);
        
        void Scale(float32 x, float32 y);
        
        void Scale(float32 x);
        
        void SetRotation(float32 val);
        
        void Rotate(float32 delta);
        
        void SetParent(const GameObject *parent);

    private:
        GameObject *m_gameobject;
        
    protected:
        BitFlag m_flags;
        /*
         * flags
         * 0 : isAvailable (default : true)
         * 1 : isStarted
         */
    };

    template<class T>
    Component *ComponentDefaultFactory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator, GameObject *gameobject)
    {
        assert(gameobject);
        Component *result = new (allocator.Alloc<T>()) T();
        assert(result);
        result->SetGameObject(gameobject);
        result->InitWithJson(obj, allocator);
        return result;
    }

    class ComponentFactory : public Storage<Component::FactoryFunc>
    {
    public:
        ComponentFactory() : Storage<Component::FactoryFunc>() { ; }

        ComponentFactory(const ComponentFactory &other) = delete;

        ~ComponentFactory() { Clear(); }

        ComponentFactory &operator=(const ComponentFactory) = delete;

        template <typename T>
        void AddFunction()
        {
            Register(&ComponentDefaultFactory<T>, T::ID);
        }

        template <typename T>
        void AddFunction(Component::FactoryFunc func)
        {
            Register(func, T::ID);
        }

        template <typename T>
        Component::FactoryFunc *GetFunction()
        {
            return (Get(T::ID));
        }

        Component::FactoryFunc *GetFunction(Uint32 id)
        {
            return (Get(id));
        }

        static ComponentFactory &GetGlobal()
        {
            static ComponentFactory instance;
            return instance;
        }
    };

}

#endif