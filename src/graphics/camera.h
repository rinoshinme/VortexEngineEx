#pragma once

#include <glm/glm.hpp>

namespace vortex
{
    // Camera Implementation using Euler Angles.
    class Camera
    {
    private:
        float m_Yaw = -180.0f;
        float m_Pitch = 0.0f;
        float m_Zoom = 45.0f;  // (half) fov of the camera
        glm::vec3 m_Position;
        glm::vec3 m_Up;
        glm::vec3 m_WorldUp; // (0, 1, 0)

        // derived parameters
        glm::vec3 m_Front;
        glm::vec3 m_Right;
    public:
        Camera();
        Camera(const glm::vec3& pos, const glm::vec3& up, float yaw, float pitch);
        glm::mat4 GetViewMatrix() const;

        void Zoom(float dz);
        void Translate(float dx, float dy, float dz);
        void Rotate(float dyaw, float dpitch);
        void MoveTo(const glm::vec3& pos);
        void PointTo(const glm::vec3& center);
        void Home();

    private:
        void UpdateVectors();
    };

    // Camera Implementation using Quaternion
    class CameraQuat
    {
    private:
    
    public:
        CameraQuat();
    };

}
