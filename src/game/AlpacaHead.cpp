#include "AlpacaHead.hpp"
#include "Alpaca.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>

namespace Game
{
    namespace Alpaca
    {
        Head::Head()
        :Sprite(), m_alpaca(nullptr), m_necks(), m_necksprites(),
        m_headpos(), m_neckpos()
        {
            ;
        }

        Head::~Head()
        {
            ;
        }

        void Head::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
        {
            Sprite::InitWithJson(obj, allocator);

            assert(obj.HasMember("headpos"));
            assert(obj.HasMember("neckpos"));
            assert(obj.HasMember("neck"));
            assert(obj["headpos"].IsObject());
            assert(obj["neckpos"].IsObject());
            assert(obj["neck"].IsObject());

            JsonParseMethods::ReadVector(obj["headpos"].GetObject(), &m_headpos);
            JsonParseMethods::ReadVector(obj["neckpos"].GetObject(), &m_neckpos);

            auto neckobj = obj["neck"].GetObject();
            for(int i=0; i<m_necks.GetMaxSize(); ++i)
            {
                m_necks[i].InitWithJson(neckobj, allocator);
                m_necksprites[i] = m_necks[i].GetComponent<Sprite>();
            }
        }

        void Head::Start()
        {
            GetGameObject()->SetLocalPosition(m_headpos);
            for(int i=0; i<m_necks.GetMaxSize(); ++i)
            {
                m_necks[i].Start();
                m_necks[i].SetLocalPosition(m_neckpos);
            }
            Sprite::Start();
        }

        void Head::Update()
        {
            Sprite::Update();
            for(int i=0; i<m_necks.GetMaxSize(); ++i)
            {
                m_necks[i].Update();
            }
        }

        void Head::Release()
        {
            for(int i=0; i<m_necks.GetMaxSize(); ++i)
            {
                m_necks[i].Release();
                m_necks[i].~ComponentObject();
            }
            Sprite::Release();
        }

        void Head::Draw()
        {
            for(int i=0; i<m_necks.GetMaxSize(); ++i)
            {
                m_necksprites[i]->Draw();
            }
            Sprite::Draw();
        }

        void Head::ResetPosition()
        {
            GameObject *gameobject = GetGameObject();
            gameobject->SetLocalPosition(m_headpos);
            gameobject->SetRotation(0.0f);
            for(int i=0; i<m_necks.GetMaxSize(); ++i)
                m_necks[i].SetLocalPosition(m_neckpos);
        }

        void Head::SetAlpaca(Alpaca *alpaca)
        {
            assert(alpaca);
            m_alpaca = alpaca;
        }

        void Head::SetHeadPos(const glm::vec3 &pos)
        {
            for(int i=0; i<m_necks.GetMaxSize(); ++i)
            {
                m_necks[i].SetLocalPosition(glm::mix(m_neckpos, pos, (float32)i/(float32)(m_necks.GetMaxSize())));
            }
            GetGameObject()->SetLocalPosition(pos);
        }

        GameObject &Head::GetNeck(int32 i)
        {
            return m_necks[i];
        }

        Uint32 Head::GetNeckCount()const
        {
            return m_necks.GetMaxSize();
        }

        const glm::vec3 &Head::GetHeadPos()const
        {
            return m_headpos;
        }

        const glm::vec3 &Head::GetNeckPos()const
        {
            return m_neckpos;
        }
    }
}