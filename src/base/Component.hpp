#ifndef BaseComponent_hpp
#define BaseComponent_hpp

#include "GameObject.hpp"

#define COMPONENT(__TYPENAME__)     \
MAKE_TYPE_ID(__TYPENAME__);         \
virtual Uint32 GetTypeID()const  { return ID; }

namespace Base
{
    class ComponentObject;

    class Component 
    {
    public:
        typedef Component* (FactoryFunc)(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator);

        static Component *Factory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator);

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

        ComponentObject *GetGameObject();

        const ComponentObject *GetGameObject()const;

        int32 isAvailable()const;

        int32 isStarted()const;

        void SetGameObject(ComponentObject *gameobject);

        void SetAvailable(int32 val);

    private:
        ComponentObject *m_gameobject;
        
    protected:
        BitFlag m_flags;
        /*
         * flags
         * 0 : isAvailable (default : true)
         * 1 : isStarted
         */
    };

    template<class T>
    Component *ComponentDefaultFactory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator)
    {
        Component *result = new (allocator.Alloc<T>()) T();
        assert(result);
        result->InitWithJson(obj, allocator);
        return result;
    }

    class ComponentObject : public GameObject
    {
    public:
        GAMEOBJECT(ComponentObject);

        ComponentObject();

        ComponentObject(Uint32 id, int32 isStatic = false);

        ComponentObject(Uint32 id, const GameObject *parent, int32 isStatic = false);

        // not copying components
        ComponentObject(const ComponentObject &other);

        virtual ~ComponentObject();

        // not copying components
        ComponentObject &operator=(const ComponentObject &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual void Start();

        virtual void Update();

        virtual void Release();

        template<class T>
        T *GetComponent()
        {
            for(Uint32 i=0; i<m_componentcount; ++i)
            {
                if(T::ID == m_components[i]->GetTypeID())
                    return static_cast<T*>(m_components[i]);
            }
            return nullptr;
        }

        template<class T>
        const T *GetComponent()const
        {
            for(Uint32 i=0; i<m_componentcount; ++i)
            {
                if(T::ID == m_components[i]->GetTypeID())
                    return static_cast<T*>(m_components[i]);
            }
            return nullptr;
        }

        Uint32 GetComponentCount()const;

    private:
        Uint32 m_componentcount;
        Component **m_components;
    };

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