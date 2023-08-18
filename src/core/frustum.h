#pragma once

#include <glm/glm.hpp>

namespace vortex
{
    struct Frustum
    {
    public:
        virtual glm::mat4 GetProjectionMatrix() const = 0;
    };

    struct PerspectiveFrustum : public Frustum
    {
    private:
        float m_Fov;
        float m_Near;
        float m_Far;
        int m_WindowWidth;
        int m_WindowHeight;

    public:
        PerspectiveFrustum(float fov, float near, float far, int width, int height)
            : m_Fov(fov), m_Near(near), m_Far(far), m_WindowWidth(width), m_WindowHeight(height)
        {}

        void SetWindowSize(int width, int height)
        {
            m_WindowWidth = width;
            m_WindowHeight = height;
        }

        virtual glm::mat4 GetProjectionMatrix() const override
        {
            float ratio = (float)m_WindowWidth / (float)m_WindowHeight;
            return glm::perspective(m_Fov, ratio, m_Near, m_Far);
        }
    };

    struct OrthogonalFrustum : public Frustum
    {
    private:
        float m_Xmin;
        float m_Xmax;
        float m_Ymin;
        float m_Ymax;
        float m_Zmin;
        float m_Zmax;

    public:
        OrthogonalFrustum(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
            : m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax), m_Zmin(zmin), m_Zmax(zmax) {}

        virtual glm::mat4 GetProjectionMatrix() const override
        {
            return glm::ortho(m_Xmin, m_Xmax, m_Ymin, m_Ymax, m_Zmin, m_Zmax);
        }
    };
}
