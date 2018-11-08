#ifndef GameGameManager_hpp
#define GameGameManager_hpp

#include "base.hpp"
#include "Alpaca.hpp"

using namespace Base;

namespace Game
{
    class GameManager: public Component
    {
    public:
        COMPONENT(GameManager);

        GameManager();

        GameManager(GameManager &other) = delete;

        virtual ~GameManager();

        GameManager &operator=(const GameManager &other) = delete;

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual void Start();

        virtual void Update();

        virtual void Release();

        Alpaca::Alpaca *GetAlpaca(Uint32 idx);

    private:
        static GameManager *global;
    public:
        static GameManager *GetGlobal();

    private:
        Alpaca::Alpaca *m_alpacas[2];
        float32 m_respawntime;
        Uint32 m_lifecount;
        Uint32 m_remainlife[2];
    };

    
    
}


#endif