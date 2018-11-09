#include "Camera.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_STATIC
#include <glm/gtx/transform.hpp>

namespace Base
{
    // Camera class

    Camera::Camera()
    :m_view(1.0f), m_projection(1.0f), 
    m_left_top_near(0.0f), m_right_bottom_far(0.0f)
    {
        m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), 
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f));
    }
    
    Camera::Camera(const Camera &other)
    :m_view(other.m_view), m_projection(other.m_projection),
    m_left_top_near(other.m_left_top_near), m_right_bottom_far(other.m_right_bottom_far)
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
        this->m_left_top_near = other.m_left_top_near;
        this->m_right_bottom_far = other.m_right_bottom_far;
        return (*this);
    }

    void Camera::InitWithJson(const rapidjson::Value::Object &obj)
    {
        assert(obj.HasMember("name"));
        assert(obj.HasMember("left_top_near"));
        assert(obj.HasMember("right_bottom_far"));
        assert(obj["name"].IsString());
        assert(obj["left_top_near"].IsObject());
        assert(obj["right_bottom_far"].IsObject());

        const char *name = obj["name"].GetString();
        StringID id(name, strlen(name));
        m_id = (Uint32)id;
        JsonParseMethods::ReadVector(obj["left_top_near"].GetObject(), &m_left_top_near);
        JsonParseMethods::ReadVector(obj["right_bottom_far"].GetObject(), &m_right_bottom_far);
        
        const glm::vec3 &ltn = m_left_top_near;
        const glm::vec3 &rbf = m_right_bottom_far;
        SetProjection(ltn.x, rbf.x, rbf.y, ltn.y, ltn.z, rbf.z);
    }

    void Camera::SetProjection(float32 left, float32 right,
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

    Uint32 Camera::GetID()const
    {
        return m_id;
    }
    
    const glm::mat4 &Camera::GetViewMatrix()const
    {
        return m_view;
    }
    
    const glm::mat4 &Camera::GetProjectionMatrix()const
    {
        return m_projection;
    }

    const glm::vec3 &Camera::GetLeftTopNear()const
    {
        return m_left_top_near;
    }

    const glm::vec3 &Camera::GetRightBottomFar()const
    {
        return m_right_bottom_far;
    }
}

