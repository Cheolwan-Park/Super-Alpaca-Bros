#ifndef GameGround_hpp
#define GameGround_hpp

#include "base.hpp"

using namespace Base;

namespace Game
{
    class Ground : public BoxCollider
    {

    public:
        COMPONENT(Ground);

        Ground();

        Ground(const Ground &other);

        virtual ~Ground();

        Ground &operator=(const Ground &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual void OnTriggerStay(Collider *other);

        virtual void OnTriggerExit(Collider *other);

        void Pass(GameObject *obj);

        int32 isPassable()const;

        void SetPassable(int32 val);

    private:
        Uint32 m_ignorelistsize;
        Collider **m_ignorelist;
    /*
     * m_flags
     * BoxCollider's flags
     * 3 : isPassable
     */
    };
}


#endif