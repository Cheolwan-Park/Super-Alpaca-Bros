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
        assert(obj.HasMember("alpaca"));
        assert(obj["respawntime"].IsFloat());
        assert(obj["lifecount"].IsInt());
        assert(obj["alpaca"].IsObject());

        m_respawntime = obj["respawntime"].GetFloat();
        m_lifecount = obj["lifecount"].GetInt();
        m_remainlife[0] = m_remainlife[1] = m_lifecount;
        auto alpacajson = obj["alpaca"].GetObject();

        Scene *scene = Application::Get().GetScene();
        assert(scene);

        GameObject *newalpaca = nullptr;
        newalpaca = scene->CreateGameObject(alpacajson);
        assert(newalpaca);
        m_alpacas[0] = newalpaca->GetComponent<Alpaca::Alpaca>();
        assert(m_alpacas[0]);
        m_alpacas[0]->SetKeymap(Alpaca::Alpaca::Keymap::MAP1);
        
        newalpaca = scene->CreateGameObject(alpacajson);
        assert(newalpaca);
        m_alpacas[1] = newalpaca->GetComponent<Alpaca::Alpaca>();
        assert(m_alpacas[1]);
        m_alpacas[1]->SetKeymap(Alpaca::Alpaca::Keymap::MAP2);
        glm::vec3 alpacapos(0.0f);
        newalpaca->GetWorldPosition(&alpacapos);
        alpacapos.x *= -1.0f;
        newalpaca->SetWorldPosition(alpacapos);
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