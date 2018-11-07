#ifndef GameHitGauge_hpp
#define GameHitGauge_hpp

#include "base.hpp"

using namespace Base;

namespace Game
{
    class HitGauge: public Component
    {
    public:
        COMPONENT(HitGauge);

        HitGauge();

        HitGauge(const HitGauge &other);

        virtual ~HitGauge();

        HitGauge &operator=(const HitGauge &other);

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual void Start();

        virtual void Update();

        virtual void Release();

        void ResetGauge();

        void GaugeUp(float32 val);

        float32 GetGauge()const;

        float32 GetFactor()const;

    private:
        float32 m_gauge;
        float32 m_maxgauge;
        float32 m_minfactor;
        float32 m_maxfactor;
    };
}

#endif