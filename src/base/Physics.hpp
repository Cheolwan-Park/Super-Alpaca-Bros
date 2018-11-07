#ifndef BasePhysics_hpp
#define BasePhysics_hpp

#include "Json.hpp"
#include "Component.hpp"

namespace Base
{
    class Collider;

    class Physics
    {
    public:
        Physics();

        Physics(const Physics &other) = delete;

        ~Physics();

        Physics &operator=(const Physics &other) = delete;

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        void Update();

        void CheckDeleted();

        Collider *Register(Collider *collider);

        float32 GetGravity()const;

        void SetGravity(float32 a);

        Uint32 GetColliderCount()const;

        Uint32 GetMaxColliderCount()const;

    private:
        float32 m_gravity;
        Uint32 m_collidercount;
        Uint32 m_maxcollidercount;
        Collider **m_colliders;
        byte *m_collidemap;
    };

    class Rigidbody : public Component
    {
    public:
        COMPONENT(Rigidbody);

        Rigidbody();

        Rigidbody(const Rigidbody &other);

        virtual ~Rigidbody();

        Rigidbody &operator=(const Rigidbody &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual void Start();

        virtual void Update();

        virtual void Release();

        virtual void OnColliderEnter(Collider *other);

        void Accelerate(const glm::vec3 &a);

        void Accelerate(float32 x, float32 y, float32 z);

        void AddForce(const glm::vec3 &f);

        void AddForce(float32 x, float32 y, float32 z);

        // get
        float32 GetMass()const;

        const glm::vec3 &GetVelocity()const;

        int32 isSimulated()const;

        int32 isGravityActive()const;

        // set
        void SetMass(float32 m);

        void SetVelocity(const glm::vec3 &v);

        void SetVelocity(float32 x, float32 y, float32 z);

        void SetSimulated(int32 val);

        void SetGravityActive(int32 val);

    private:
        float32 m_mass;
        float32 m_drag;
        glm::vec3 m_velocity;
        
        /*
         * flags
         * Component's flags
         * 2 : isSimulated
         * 3 : gravity active
         */
    };
}


#endif