#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

namespace vortex
{
    Camera::Camera()
    {
        m_Position = glm::vec3(0.0f, 0.0f, 2.0f);
        m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_Yaw = -180.0f;
        m_Pitch = 0.0f;
        UpdateVectors();
    }

    Camera::Camera(const glm::vec3& pos, const glm::vec3& up, float yaw, float pitch)
    {
        m_Position = pos;
        m_Up = up;
        m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_Yaw = yaw;
        m_Pitch = pitch;
        UpdateVectors();
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }

    glm::mat4 Camera::GetViewMatrixOrigin() const
    {
        glm::vec3 pos(0.0f, 0.0f, 0.0f);
        return glm::lookAt(pos, pos + m_Front, m_Up);
    }

    void Camera::UpdateVectors()
    {
        glm::vec3 front;
        front.x = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }

    void Camera::Zoom(float dz)
    {
        m_Zoom += dz;
        if (m_Zoom > 75.0f)
            m_Zoom = 75.0f;
        if (m_Zoom < 5.0f)
            m_Zoom = 5.0f;
    }

    void Camera::Translate(float dx, float dy, float dz)
    {
        m_Position += dx * m_Right;
        m_Position += dy * m_Up;
        m_Position += dz * m_Front;
        // translation does not affect view matrix
    }

    void Camera::Rotate(float dyaw, float dpitch)
    {
        m_Yaw += dyaw;
        m_Pitch += dpitch;
        // constrain pitch
        if (m_Pitch > 89.0f) m_Pitch = 89.0f;
        if (m_Pitch < -89.0f) m_Pitch = -89.0f;
        UpdateVectors();
    }

    void Camera::MoveTo(const glm::vec3& pos)
    {
        m_Position = pos;
    }

    void Camera::PointTo(const glm::vec3& center)
    {
        glm::vec3 dist = center - m_Position;
        // calculat yaw and pitch from dist
        float distance = glm::distance(center, m_Position);
        float dx = dist.x;
        float dy = dist.y;
        float dz = dist.z;
        float pitch = asin(dy / distance);
        m_Pitch = glm::degrees(pitch);
        float yaw = atan2(dx, dz);
        m_Yaw = glm::degrees(yaw);

        UpdateVectors();
    }
    
    void Camera::Home()
    {
        MoveTo(glm::vec3(0.0f, 0.0f, 2.0f));
        PointTo(glm::vec3(0.0f, 0.0f, 0.0f));
    }
}
