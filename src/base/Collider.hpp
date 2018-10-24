#ifndef BaseCollider_hpp
#define BaseCollider_hpp


#include "GameObject.hpp"

namespace Base 
{
    class Collider
    {
    public:
        enum class Type : Uint32  { NONE = 0, CIRCLE = 1, BOX = 2 };
    public:
        Collider();

        Collider(const Collider &other);

        virtual ~Collider();

        Collider &operator=(const Collider &other);

        virtual int32 isCollideWith(const Collider &other)const = 0;

        virtual Type GetType()const = 0;
    };

    class CircleCollider : Collider
    {
    public:
        CircleCollider();

        CircleCollider(const CircleCollider &other);

        virtual ~CircleCollider();

        CircleCollider &operator=(const CircleCollider &other);

        virtual int32 isCollideWith(const Collider &other)const;

        virtual Type GetType()const;
    };
}


#endif