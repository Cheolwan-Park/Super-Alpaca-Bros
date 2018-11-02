#ifndef BaseGameObject_hpp
#define BaseGameObject_hpp

#include "types.hpp"
#include "Math.hpp"
#include "Array.hpp"
#include "String.hpp"
#include "BitFlag.hpp"
#include "Storage.hpp"
#include "Json.hpp"
#include "Component.hpp"
#include <glm/glm.hpp>


namespace Base
{
    class Schedule;
    class ObjectStorage;
    class Component;
    class Collider;
    
    class GameObject
    {
    public:
        static GameObject *Factory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator, ::Base::ObjectStorage *storage, ::Base::Uint32 id);
    
    public:
        GameObject();
        
        GameObject(Uint32 id, int32 isStatic = false);
        
        GameObject(Uint32 id, const GameObject *parent, int32 isStatic = false);

        // not copying parent, id, isDeleted, isIdSet, isStarted, components
        GameObject(const GameObject &other);
        
        ~GameObject();
        
        // not copying parent, id, isDeleted, isIdSet, isStarted, components
        GameObject &operator=(const GameObject &other);

        void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);
        
        void Start();
        
        void Update();

        void Release();
        
        GameObject *AddChild(GameObject *child)const;

        // collide callback
        void OnColliderEnter(const Collider *other);
        void OnColliderStay(const Collider *other);
        void OnColliderExit(const Collider *other);
        
        // get
        Uint32 GetID()const;

        Uint32 GetTag()const;
        
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
        
        // set
        void SetID(Uint32 id);

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
        
        void Delete();
        
        void SetAvailable(int32 value);
        
        
    private:
        Uint32 m_id;
        Uint32 m_tag;
        Uint32 m_componentcount;
        float32 m_rotation;
        Component **m_components;
        const GameObject *m_parent;     // worldpos = m_position + m_parent.GetWorldPos()
        glm::vec3 m_position;
        glm::vec2 m_scale;
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
    
    class ObjectStorage
    {
    public:
        typedef GameObject* Type;
        
        ObjectStorage() = delete;
        
        ObjectStorage(Uint32 id, Uint32 order);
        
        ObjectStorage(const ObjectStorage &other) = delete;
        
        ~ObjectStorage();
        
        ObjectStorage &operator=(const ObjectStorage &other) = delete;

        void AssignMemory(void *mem, Uint32 len);

        GameObject *Register(GameObject *gameobject);

        GameObject *Get(Uint32 hash);
        
        void UpdateObjects();
        
        void CheckAndDelete();

        void Clear();
        
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
        Uint32 m_len;
        Type *m_gameobjects;
    };
}


#endif
