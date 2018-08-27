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
    

namespace Base
{
    class Schedule;
    class Component;
    
    class GameObject
    {
    public:
        MAKE_TYPE_ID(GameObject);

        GameObject() = delete;
        
        GameObject(Uint32 id, int32 isStatic = false);
        
        GameObject(Uint32 id, const GameObject *parent, int32 isStatic = false);
        
        GameObject(const rapidjson::GenericObject<true, rapidjson::Value> &obj);

        GameObject(const GameObject &other);
        
        virtual ~GameObject();
        
        GameObject &operator=(const GameObject &other);
        
        virtual void Start();
        
        virtual void Update();
        
        GameObject *AddChild(GameObject *child)const;
        
        // get
        Uint32 GetID()const;
        
        const glm::vec3 &GetLocalPosition()const;
        
        const glm::vec3 GetWorldPosition()const;
        
        const GameObject *GetParent()const;
        
        int32 isDeleted()const;
        
        int32 isAvailable()const;
        
        int32 isStarted()const;
        
        int32 isStatic()const;
        
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
        
        void SetParent(const GameObject *parent);
        
        void Delete();
        
        void SetAvailable(int32 value);
        
    private:
        Uint32 m_id;
        glm::vec3 m_position;
        const GameObject *m_parent;     // worldpos = m_position + m_parent.GetWorldPos()

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
}


#endif
