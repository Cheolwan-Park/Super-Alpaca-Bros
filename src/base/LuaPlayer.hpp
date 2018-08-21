#ifndef BaseLuaPlayer_hpp
#define BaseLuaPlayer_hpp

#include "types.hpp"
#include "luaclasses.hpp"
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace Base
{
    class LuaPlayer
    {
        private:
            LuaPlayer();

            LuaPlayer(const LuaPlayer &other) = delete;

            LuaPlayer &operator=(const LuaPlayer &other) = delete;
        
        public:
            ~LuaPlayer();

            int32 Init();

            lua_State *GetState();

            static LuaPlayer &Get();

        private:
            // add c++ objects to lua with luabridge
            void AddObjects();

        private:
            lua_State *m_L;
    };
}


#endif