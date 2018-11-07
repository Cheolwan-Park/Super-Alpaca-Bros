#ifndef BaseCollider_hpp
#define BaseCollider_hpp


#include "GameObject.hpp"

namespace Base 
{
    class Rigidbody;

    class Collider : public Component
    {
    public:
        enum class Type : Uint32  { NONE = 0, CIRCLE = 1, BOX = 2 };

        static Component* Factory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator, GameObject *gameobject);

    public:
        COMPONENT(Collider);

        Collider();

        Collider(const Collider &other);

        virtual ~Collider();

        Collider &operator=(const Collider &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual void Start();

        virtual void Update();

        virtual void Release();

        virtual int32 isCollideWith(const Collider *other)const = 0;

        virtual Type GetType()const = 0;

        Rigidbody *GetRigidbody();

        const Rigidbody *GetRigidbody()const;

        int32 isTrigger()const;

        void SetTrigger(int32 val);

    private:
        Rigidbody *m_rigidbody;
        
        /*
         * flags 
         * Component's flags
         * 2 : isTrigger
         */
    };

    class CircleCollider : public Collider
    {
    public:
        COMPONENT(CircleCollider);

        CircleCollider();

        CircleCollider(const CircleCollider &other);

        virtual ~CircleCollider();

        CircleCollider &operator=(const CircleCollider &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual int32 isCollideWith(const Collider *other)const;

        virtual Collider::Type GetType()const;

        // get 
        float32 GetRadius()const;

        // set
        void SetRadius(float32 r);

    private:
        float32 m_radius;
    };

    class BoxCollider : public Collider
    {
    public:
        COMPONENT(BoxCollider);

        BoxCollider();

        BoxCollider(const BoxCollider &other);

        virtual ~BoxCollider();

        BoxCollider &operator=(const BoxCollider &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual int32 isCollideWith(const Collider *other)const;

        virtual Collider::Type GetType()const;

        // get
        const Math::Rect &GetBox()const;

        // set
        void SetBox(const Math::Rect &box);

    private:
        Math::Rect m_box;
    };

    namespace CollideCheckFunctions
    {
        int32 isCollide(const CircleCollider *circle0, const CircleCollider *circle1);
        int32 isCollide(const BoxCollider *box, const CircleCollider *circle);
        int32 isCollide(const BoxCollider *box0, const BoxCollider *box1);
    }
}


#endif