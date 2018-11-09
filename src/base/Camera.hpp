#ifndef BaseCamera_hpp
#define BaseCamera_hpp

#include "types.hpp"
#include "Json.hpp"
#ifdef _WIN32
#include <GL/gl.h>
#define GLM_ENABLE_EXPERIMENTAL
#endif
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif
#include <glm/glm.hpp>

namespace Base
{
    class Camera
    {
    public:
        Camera();
        
        Camera(const Camera &other);
        
        virtual ~Camera();
        
        Camera &operator=(const Camera &other);

        void InitWithJson(const rapidjson::Value::Object &obj);
    
        void SetProjection(float32 left, float32 right,
                           float32 bottom, float32 top,
                           float32 near, float32 far);

        Uint32 GetID()const;
        
        const glm::mat4 &GetViewMatrix()const;
        
        const glm::mat4 &GetProjectionMatrix()const;
        
        const glm::vec3 &GetLeftTopNear()const;
        
        const glm::vec3 &GetRightBottomFar()const;
        
    private:
        Uint32 m_id;
        glm::mat4 m_view;
        glm::mat4 m_projection;
        glm::vec3 m_left_top_near;
        glm::vec3 m_right_bottom_far;
    };
}

#endif
