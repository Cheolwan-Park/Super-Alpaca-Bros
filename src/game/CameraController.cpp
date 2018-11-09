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

        assert(obj.HasMember("isSmooth"));
        assert(obj.HasMember("margin"));
        assert(obj.HasMember("cammovespeed"));
        assert(obj.HasMember("expandspeed"));
        assert(obj.HasMember("viewratio"));
        assert(obj.HasMember("min_camerarange"));
        assert(obj.HasMember("max_camerarange"));
        assert(obj["isSmooth"].IsBool());
        assert(obj["margin"].IsFloat());
        assert(obj["expandspeed"].IsFloat());
        assert(obj["cammovespeed"].IsFloat());
        assert(obj["viewratio"].IsObject());
        assert(obj["min_camerarange"].IsFloat());
        assert(obj["max_camerarange"].IsFloat());
        
        SetSmooth(obj["isSmooth"].GetBool());
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
        auto *scene = Application::Get().GetScene();
        Camera *camera = scene->GetCamera("main"_hash);
        Time &t = Time::Get();
        const glm::vec3 &ltn = camera->GetLeftTopNear();
        const glm::vec3 &rbf = camera->GetRightBottomFar();
        
        // move cam
        glm::vec3 pos0(0.0f), pos1(0.0f);
        Alpaca::Alpaca *alpaca0 = m_gmanager->GetAlpaca(0);
        Alpaca::Alpaca *alpaca1 = m_gmanager->GetAlpaca(1);
        m_gmanager->GetAlpaca(0)->GetWorldPosition(&pos0);
        m_gmanager->GetAlpaca(1)->GetWorldPosition(&pos1);
        glm::vec3 newcampos(0.0f);
        if(alpaca0->isAvailable() && alpaca1->isAvailable()) { newcampos = (pos0+pos1)/2.0f; }
        else if(alpaca0->isAvailable()) { newcampos = pos0; }
        else if(alpaca1->isAvailable()) { newcampos = pos1; }
        else { newcampos.x = newcampos.y = 0.0f; }
        glm::vec3 campos = (ltn+rbf)/2.0f;
        newcampos.z = campos.z = 1.0f;
        glm::vec3 delta = newcampos - campos;
        float32 movedistance = t.GetDeltatime()*m_cammovespeed;
        if(isSmooth())
        {
            movedistance *= (-exp(-(delta.x*delta.x + delta.y*delta.y))+1.0f);
        }
        if((delta.x*delta.x+delta.y*delta.y) > movedistance)
        {
            campos += glm::normalize(delta) * movedistance;
        }
        
        // setting camera view size
        glm::vec2 camerarange = {0.0f, 0.0f};
        camerarange.x = (Math::Abs(pos0.x - pos1.x) + 2*m_margin) / m_viewratio.x;
        camerarange.y = (Math::Abs(pos0.y - pos1.y) + 2*m_margin) / m_viewratio.y;
        float32 nowrange = (m_mincamerarange + (m_maxcamerarange-m_mincamerarange)*m_nowexpanded);
        float32 targetrange = camerarange.x > camerarange.y ? camerarange.x : camerarange.y;
        float32 expandpercentage = m_expandspeed * t.GetDeltatime();
        if(isSmooth())
        {
            expandpercentage *= (-exp(-(targetrange-nowrange)*(targetrange-nowrange))+1.0f);
        }
        float32 expand = (m_maxcamerarange-m_mincamerarange)*expandpercentage;
        expand = nowrange > targetrange ? -expand : expand;
        if(targetrange > nowrange && targetrange > nowrange + expand)
        {
            m_nowexpanded += expandpercentage;
        }
        else if(targetrange < nowrange && targetrange < nowrange - expand)
        {
            m_nowexpanded -= expandpercentage;
        }
        m_nowexpanded = m_nowexpanded < 0.0f ? 0.0f : m_nowexpanded;
        m_nowexpanded = m_nowexpanded > 1.0f ? 1.0f : m_nowexpanded;
        nowrange = (m_mincamerarange + (m_maxcamerarange-m_mincamerarange)*m_nowexpanded);
        glm::vec2 camviewsize = m_viewratio * nowrange;
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

    int32 CameraController::isSmooth()const
    {
        return m_flags.GetFlag(2);
    }

    void CameraController::SetSmooth(int32 val)
    {
        m_flags.SetFlag(2, val);
    }
}