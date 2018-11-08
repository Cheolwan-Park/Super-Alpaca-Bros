#include "GameManager.hpp"

namespace Game
{
    // GameManager class
    GameManager::GameManager()
    :Component(), m_alpacas(), 
    m_respawntime(0.0f), m_lifecount(0), m_remainlife()
    {
        ;
    }

    GameManager::~GameManager()
    {
        ;
    }

    void GameManager::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        Component::InitWithJson(obj, allocator);

        assert(obj.HasMember("respawntime"));
        assert(obj.HasMember("lifecount"));
        assert(obj.HasMember("alpaca0"));
        assert(obj.HasMember("alpaca1"));
        assert(obj["respawntime"].IsFloat());
        assert(obj["lifecount"].IsInt());
        assert(obj["alpaca0"].IsObject());
        assert(obj["alpaca1"].IsObject());

        m_respawntime = obj["respawntime"].GetFloat();
        m_lifecount = obj["lifecount"].GetInt();
        m_remainlife[0] = m_remainlife[1] = m_lifecount;
        auto alpaca0json = obj["alpaca0"].GetObject();
        auto alpaca1json = obj["alpaca1"].GetObject();

        Scene *scene = Application::Get().GetScene();
        assert(scene);

        GameObject *newalpaca = nullptr;
        newalpaca = scene->CreateGameObject(alpaca0json);
        assert(newalpaca);
        m_alpacas[0] = newalpaca->GetComponent<Alpaca::Alpaca>();
        assert(m_alpacas[0]);
        m_alpacas[0]->SetKeymap(Alpaca::Alpaca::Keymap::MAP1);
        
        newalpaca = scene->CreateGameObject(alpaca1json);
        assert(newalpaca);
        m_alpacas[1] = newalpaca->GetComponent<Alpaca::Alpaca>();
        assert(m_alpacas[1]);
        m_alpacas[1]->SetKeymap(Alpaca::Alpaca::Keymap::MAP2);
    }

    void GameManager::Start()
    {
        assert(!global);
        global = this;
    }

    void GameManager::Update()
    {
        ;
    }

    void GameManager::Release()
    {
        ;
    }

    Alpaca::Alpaca *GameManager::GetAlpaca(Uint32 idx)
    {
        assert(idx<2);
        return m_alpacas[idx];
    }

    GameManager *GameManager::global = nullptr;
    GameManager *GameManager::GetGlobal()
    {
        return global;
    }
}