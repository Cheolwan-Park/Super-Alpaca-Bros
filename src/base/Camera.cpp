#include "Camera.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_STATIC
#include <glm/gtx/transform.hpp>

namespace Base
{
    // Camera class
    
    Camera *Camera::main = nullptr;
    
    Camera::Camera()
    :m_view(1.0f), m_projection(1.0f)
    {
        ;
    }
    
    Camera::Camera(const Camera &other)
    :m_view(other.m_view),
    m_projection(other.m_projection)
    {
        ;
    }
    
    Camera::~Camera()
    {
        ;
    }
    
    Camera &Camera::operator=(const Camera &other)
    {
        assert(this != &other);
        this->m_view = other.m_view;
        this->m_projection = other.m_projection;
        return (*this);
    }
    
    void Camera::SetView(const glm::vec3 &position,
                         const glm::vec3 &target,
                         const glm::vec3 &up)
    {
        m_view = glm::lookAt(position, target, up);
    }
    
    const glm::mat4 &Camera::GetViewMatrix()const
    {
        return m_view;
    }
    
    const glm::mat4 &Camera::GetProjectionMatrix()const
    {
        return m_projection;
    }
    
    Camera *Camera::GetMain()
    {
        return main;
    }
    
    void Camera::SetMain(Camera *cam)
    {
        main = cam;
    }
    
    
    // OrthoCamera
    
    OrthoCamera::OrthoCamera()
    :Camera(), m_left_top_near(0.0f), m_right_bottom_far(0.0f)
    {
        ;
    }
    
    OrthoCamera::OrthoCamera(const OrthoCamera &other)
    :Camera(other), m_left_top_near(other.m_left_top_near),
    m_right_bottom_far(other.m_right_bottom_far)
    {
        
    }
    
    OrthoCamera::~OrthoCamera()
    {
        ;
    }
    
    OrthoCamera &OrthoCamera::operator=(const OrthoCamera &other)
    {
        Camera::operator=(other);
        assert(this != &other);
        this->m_left_top_near = other.m_left_top_near;
        this->m_right_bottom_far = other.m_right_bottom_far;
        return (*this);
    }
    
    void OrthoCamera::SetProjection(float32 left, float32 right,
                                    float32 bottom, float32 top,
                                    float32 near, float32 far)
    {
        m_projection = glm::ortho(left, right,
                                  bottom, top,
                                  near, far);
        m_left_top_near.x = left;
        m_left_top_near.y = top;
        m_left_top_near.z = near;
        m_right_bottom_far.x = right;
        m_right_bottom_far.y = bottom;
        m_right_bottom_far.z = far;
    }
    
    const glm::vec3 &OrthoCamera::GetLeftTopNear()const
    {
        return m_left_top_near;
    }
    
    const glm::vec3 &OrthoCamera::GetRightBottomFar()const
    {
        return m_right_bottom_far;
    }
}

