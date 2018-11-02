#ifndef BasePhysics_hpp
#define BasePhysics_hpp

#include "Json.hpp"

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

        Uint32 GetColliderCount()const;

        Uint32 GetMaxColliderCount()const;

        static Physics &GetGlobal();

    private:
        Uint32 m_collidercount;
        Uint32 m_maxcollidercount;
        Collider **m_colliders;
        byte *m_collidemap;
    };
}


#endif