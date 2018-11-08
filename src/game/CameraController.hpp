#ifndef GameCameraController_hpp
#define GameCameraController_hpp

#include "base.hpp"

using namespace Base;

namespace Game
{
    class GameManager;

    class CameraController : public Component
    {
    public:
        COMPONENT(CameraController);

        CameraController();

        CameraController(const CameraController &other) = delete;

        virtual ~CameraController();

        CameraController &operator=(const CameraController &other) = delete;

        virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

        virtual void Start();

        virtual void Update();

        virtual void Release();

        int32 isSmooth()const;

        void SetSmooth(int32 val);

    private:
        GameManager *m_gmanager;
        float32 m_margin;
        float32 m_cammovespeed;
        float32 m_expandspeed;
        float32 m_nowexpanded;
        glm::vec2 m_viewratio;
        float32 m_mincamerarange;
        float32 m_maxcamerarange;

        /*
         * flags
         * Component's flag
         * 2 : isSmooth
         */
    };
}

#endif