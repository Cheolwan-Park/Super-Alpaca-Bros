#ifndef BaseCamera_hpp
#define BaseCamera_hpp

#include "types.hpp"
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
        
        void SetView(const glm::vec3 &position,
                     const glm::vec3 &target,
                     const glm::vec3 &up);
        
        const glm::mat4 &GetViewMatrix()const;
        
        const glm::mat4 &GetProjectionMatrix()const;
        
        static Camera *GetMain();
        
        static void SetMain(Camera *cam);
        
    private:
        glm::mat4 m_view;
    protected:
        glm::mat4 m_projection;
        
    private:
        static Camera *main;
    };
    
    class OrthoCamera : public Camera
    {
    public:
        OrthoCamera();
        
        OrthoCamera(const OrthoCamera &other);
        
        virtual ~OrthoCamera();
        
        OrthoCamera &operator=(const OrthoCamera &other);
        
        void SetProjection(float32 left, float32 right,
                           float32 bottom, float32 top,
                           float32 near, float32 far);
        
        const glm::vec3 &GetLeftTopNear()const;
        
        const glm::vec3 &GetRightBottomFar()const;
        
    private:
        glm::vec3 m_left_top_near;
        glm::vec3 m_right_bottom_far;
    };
    
}

#endif
