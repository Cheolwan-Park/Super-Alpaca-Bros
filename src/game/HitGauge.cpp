#include "HitGauge.hpp"

namespace Game
{
    HitGauge::HitGauge()
    :Component(), m_gauge(0.0f), m_maxgauge(0.0f), 
    m_minfactor(0.0f), m_maxfactor(0.0f)
    {
        ;
    }

    HitGauge::HitGauge(const HitGauge &other)
    :Component(other), m_gauge(other.m_gauge), m_maxgauge(other.m_maxgauge),
    m_minfactor(other.m_minfactor), m_maxfactor(other.m_maxfactor)
    {
        ;
    }

    HitGauge::~HitGauge()
    {
        ;
    }

    HitGauge &HitGauge::operator=(const HitGauge &other)
    {
        assert(this != &other);
        Component::operator=(other);
        this->m_gauge = other.m_gauge;
        this->m_maxgauge = other.m_maxgauge;
        this->m_minfactor = other.m_minfactor;
        this->m_maxfactor = other.m_maxfactor;
        return (*this);
    }

    void HitGauge::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
    {
        Component::InitWithJson(obj, allocator);

        assert(obj.HasMember("maxgauge"));
        assert(obj.HasMember("minfactor"));
        assert(obj.HasMember("maxfactor"));
        assert(obj["maxgauge"].IsFloat());
        assert(obj["minfactor"].IsFloat());
        assert(obj["maxfactor"].IsFloat());

        m_maxgauge = obj["maxgauge"].GetFloat();
        m_minfactor = obj["minfactor"].GetFloat();
        m_maxfactor = obj["maxfactor"].GetFloat();
    }

    void HitGauge::Start()
    {
        Component::Start();
    }

    void HitGauge::Update()
    {
        ;
    }

    void HitGauge::Release()
    {
        ;
    }

    void HitGauge::ResetGauge()
    {
        m_gauge = 0.0f;
    }

    void HitGauge::GaugeUp(float32 val)
    {
        m_gauge += val;
        m_gauge = m_gauge > m_maxgauge ? m_maxgauge : m_gauge;
    }

    float32 HitGauge::GetGauge()const
    {
        return m_gauge;
    }

    float32 HitGauge::GetFactor()const
    {
        float32 percentage = m_gauge/m_maxgauge;
        return (m_minfactor + (m_maxfactor - m_minfactor)*percentage*percentage);
    }
}