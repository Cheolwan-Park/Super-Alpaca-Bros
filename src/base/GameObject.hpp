#ifndef BaseGameObject_hpp
#define BaseGameObject_hpp

#include "types.hpp"
#include "Math.hpp"
#include "Array.hpp"
#include "String.hpp"
#include "Schedule.hpp"
#include "BitFlag.hpp"
#include "Storage.hpp"
#include "Json.hpp"
#include <glm/glm.hpp>


#define MAKE_TYPE_ID(__TYPENAME__)  \
static constexpr Uint32 ID = ::Base::CompileTimeHash::fnv1a_32(#__TYPENAME__, sizeof(#__TYPENAME__)-1);

#define GAMEOBJECT(__TYPENAME__)    \
MAKE_TYPE_ID(__TYPENAME__);         \
static GameObject *Factory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator, ::Base::ObjectStorage *storage, ::Base::Uint32 id);    \
virtual Uint32 GetTypeID()const  { return ID; }
    

namespace Base
{
    class Schedule;
    class ObjectStorage;
    class Component;
    
    class GameObject
    {
    public:
        typedef GameObject* (FactoryFunc)(const ::rapidjson::Value::Object&, ::Base::StackAllocator&, ::Base::ObjectStorage*, ::Base::Uint32);
    
    public:
        GAMEOBJECT(GameObject);

        GameObject();
        
        GameObject(Uint32 id, int32 isStatic = false);
        
        GameObject(Uint32 id, const GameObject *parent, int32 isStatic = false);

        // not copying parent, id, isDeleted, isIdSet, isStarted
        GameObject(const GameObject &other);
        
        virtual ~GameObject();
        
        // not copying parent, id, isDeleted, isIdSet, isStarted
        GameObject &operator=(const GameObject &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);
        
        virtual void Start();
        
        virtual void Update();

        virtual void Release();
        
        GameObject *AddChild(GameObject *child)const;
        
        // get
        Uint32 GetID()const;
        
        const glm::vec3 &GetLocalPosition()const;
        
        void GetWorldPosition(glm::vec3 *vec)const;
        
        const GameObject *GetParent()const;
        
        int32 isDeleted()const;
        
        int32 isAvailable()const;
        
        int32 isStarted()const;
        
        int32 isStatic()const;

        const glm::vec2 &GetScale()const;
        
        float32 GetRotation()const;

        const glm::mat3x3 &GetLocalModel()const;

        void GetModel(glm::mat3x3 *mat)const;
        
        // set
        void SetID(Uint32 id);
        
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
        
        void Delete();
        
        void SetAvailable(int32 value);
        
        
    private:
        Uint32 m_id;
        glm::vec3 m_position;
        const GameObject *m_parent;     // worldpos = m_position + m_parent.GetWorldPos()
        glm::vec2 m_scale;
        float32 m_rotation;
        glm::mat3x3 m_model;

    protected:
        /* flags
         * 0 : isDeleted
         * 1 : isAvailable (default : true)
         * 2 : isStarted
         * 3 : isStatic
         * 4 : isIdSet
         */
        BitFlag m_flags;
    };
    
    class ObjectStorage : public Storage<GameObject>
    {
    public:
        typedef GameObject* Type;
        
        ObjectStorage() = delete;
        
        ObjectStorage(Uint32 id, Uint32 order);
        
        ObjectStorage(const ObjectStorage &other) = delete;
        
        ~ObjectStorage();
        
        ObjectStorage &operator=(const ObjectStorage &other) = delete;
        
        void UpdateObjects();
        
        void CheckAndDeleteObjects();
        
        // get
        Uint32 GetID()const;
        
        Uint32 GetOrder()const;

    private:
        static void UpdateObject(GameObject **obj);
        static void CheckAndDeleteObject(GameObject **obj);
        static void FreeObject(GameObject **obj);
        
    private:
        Uint32 m_id;
        Uint32 m_order;
    };

    class GameObjectFactory : public Storage<GameObject::FactoryFunc>
    {
    public:
        GameObjectFactory() : Storage<GameObject::FactoryFunc>() { ; }

        GameObjectFactory(const GameObjectFactory &other) = delete;

        ~GameObjectFactory() { Clear(); }

        GameObjectFactory &operator=(const GameObjectFactory) = delete;

        template <typename T>
        void AddFunction()
        {
            Register(&T::Factory, T::ID);
        }

        template <typename T>
        GameObject::FactoryFunc *GetFunction()
        {
            return (Get(T::ID));
        }

        GameObject::FactoryFunc *GetFunction(Uint32 id)
        {
            return (Get(id));
        }

        static GameObjectFactory &GetGlobal()
        {
            static GameObjectFactory instance;
            return instance;
        }
    };
}


#endif
