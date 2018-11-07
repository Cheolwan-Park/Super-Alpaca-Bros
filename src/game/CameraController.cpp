#include "CameraController.hpp"
#include "GameManager.hpp"
#include "Alpaca.hpp"

namespace Game
{
    CameraController::CameraController()
    :Component(), m_gmanager(nullptr),
    m_margin(0.0f), m_cammovespeed(0.0f), m_expandspeed(0.0f), m_nowexpanded(0.0f),
    m_viewratio(), m_mincamerarange(0.0f), m_maxcamerarange(0.0f)
    {
        ;
    }

    CameraController::~CameraController()
    {
        ;
    }

    void CameraController::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        Component::InitWithJson(obj, allocator);

        assert(obj.HasMember("margin"));
        assert(obj.HasMember("cammovespeed"));
        assert(obj.HasMember("expandspeed"));
        assert(obj.HasMember("viewratio"));
        assert(obj.HasMember("min_camerarange"));
        assert(obj.HasMember("max_camerarange"));
        assert(obj["margin"].IsFloat());
        assert(obj["expandspeed"].IsFloat());
        assert(obj["cammovespeed"].IsFloat());
        assert(obj["viewratio"].IsObject());
        assert(obj["min_camerarange"].IsFloat());
        assert(obj["max_camerarange"].IsFloat());

        m_margin = obj["margin"].GetFloat();
        m_cammovespeed = obj["cammovespeed"].GetFloat();
        m_expandspeed = obj["expandspeed"].GetFloat();
        JsonParseMethods::ReadVector2(obj["viewratio"].GetObject(), &m_viewratio);
        m_mincamerarange = obj["min_camerarange"].GetFloat();
        m_maxcamerarange = obj["max_camerarange"].GetFloat();
    }

    void CameraController::Start()
    {
        m_gmanager = GetGameObject()->GetComponent<GameManager>();
    }

    void CameraController::Update()
    {
        assert(m_gmanager);

        // fitting camera
        OrthoCamera *camera = (OrthoCamera*)Camera::GetMain();
        
        glm::vec3 pos0(0.0f), pos1(0.0f);
        m_gmanager->GetAlpaca(0)->GetWorldPosition(&pos0);
        m_gmanager->GetAlpaca(1)->GetWorldPosition(&pos1);

        const glm::vec3 &ltn = camera->GetLeftTopNear();
        const glm::vec3 &rbf = camera->GetRightBottomFar();
        glm::vec3 campos = (pos0+pos1)/2.0f;
        campos.z = 0.0f;
        float32 camdistancefromo = campos.length();
        campos = glm::normalize(campos)*Math::Log(camdistancefromo);
        campos.z = 1.0f;
        
        glm::vec2 camviewsize = m_viewratio*(m_mincamerarange + (m_maxcamerarange-m_mincamerarange)*m_nowexpanded);
        glm::vec2 camerarange = {0.0f, 0.0f};
        camerarange.x = Math::Abs(pos0.x - pos1.x) + 2*m_margin;
        camerarange.y = Math::Abs(pos0.y - pos1.y) + 2*m_margin; 

        Time &t = Time::Get();
        if(camerarange.x > camviewsize.x 
        || camerarange.y > camviewsize.y)
        {
            m_nowexpanded += m_expandspeed * t.GetDeltatime();
        }
        else
        {
            m_nowexpanded -= m_expandspeed * t.GetDeltatime();
        }
        
        m_nowexpanded = m_nowexpanded < 0.0f ? 0.0f : m_nowexpanded;
        m_nowexpanded = m_nowexpanded > 1.0f ? 1.0f : m_nowexpanded;
        camviewsize = m_viewratio*(m_mincamerarange + (m_maxcamerarange-m_mincamerarange)*m_nowexpanded);
        camviewsize *= 0.5f;

        camera->SetProjection(campos.x - camviewsize.x, 
                              campos.x + camviewsize.x,
                              campos.y - camviewsize.y,
                              campos.y + camviewsize.y,
                              ltn.z, rbf.z);
    }

    void CameraController::Release()
    {
        ;
    }
}