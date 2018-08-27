#include "LuaPlayer.hpp"

namespace Base
{
    LuaPlayer::LuaPlayer()
    :m_L(nullptr)
    {
        ;
    }

    LuaPlayer::~LuaPlayer()
    {
        if(m_L)
            lua_close(m_L);
    }

    int32 LuaPlayer::Init()
    {
        if(!m_L)
        {
            m_L = luaL_newstate();
            assert(m_L);
            luaL_openlibs(m_L);
            AddObjects();
            return RET_SUCC;
        }
        return RET_FAILED;
    }

    lua_State *LuaPlayer::GetState()
    {
        return m_L;
    }

    LuaPlayer &LuaPlayer::Get()
    {
        static LuaPlayer instance;
        return instance;
    }


    // ======= add c++ objects to lua with luabridge =======
    // #include "GameObject.hpp"

    #define ADDFUNCTION(__CLASS__, __METHOD__)         \
    addFunction(#__METHOD__, &__CLASS__::__METHOD__)

    void LuaPlayer::AddObjects()
    {
        luabridge::getGlobalNamespace(m_L)
            .beginNamespace("Base")
                .beginClass<GameObject>("GameObject")
                    .ADDFUNCTION(GameObject, GetID)
                    .ADDFUNCTION(GameObject, GetLocalPosition)
                    .ADDFUNCTION(GetWorldPosition)
                    .ADDFUNCTION(GetParent)
                    .ADDFUNCTIOn(isAvailable)
                    .ADDFUNCTION(isStarted)
                    .ADDFUNCTION(isStatic)
                    .ADDFUNCTION(SetID)
                    .ADDFUNCTION(SetLocalPosition)
                    .ADDFUNCTION()
                .endClass()
            .endNamespace();
    }
}