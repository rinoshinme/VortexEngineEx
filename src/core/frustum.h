#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ray.h"

namespace vortex
{
    struct Frustum
    {
    public:
        virtual ~Frustum() {}
        virtual glm::mat4 GetProjectionMatrix() const = 0;
        virtual Ray GetCastingRay(int xpos, int ypos) const = 0;
        virtual void SetWindowSize(int width, int height) = 0;
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

        virtual void SetWindowSize(int width, int height) override
        {
            m_WindowWidth = width;
            m_WindowHeight = height;
        }

        virtual glm::mat4 GetProjectionMatrix() const override
        {
            float ratio = (float)m_WindowWidth / (float)m_WindowHeight;
            return glm::perspective(m_Fov, ratio, m_Near, m_Far);
        }

        virtual Ray GetCastingRay(int xpos, int ypos) const override
        {
            glm::vec3 pt1(0.0f, 0.0f, 0.0f);
            float y_range = m_Near * tan(m_Fov / 2);
            float y = y_range * (m_WindowHeight / 2 - float(ypos)) / (m_WindowHeight / 2);
            float x_range = y_range * m_WindowWidth / m_WindowHeight;
            float x = x_range * (float(xpos) - m_WindowWidth / 2) / (m_WindowWidth / 2);
            glm::vec3 pt2 = glm::vec3(x, y, -m_Near);
            vortex::Ray ray(pt1, pt2);
            return ray;
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

        int m_WindowWidth;
        int m_WindowHeight;

    public:
        OrthogonalFrustum(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
            : m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax), m_Zmin(zmin), m_Zmax(zmax) 
        {
            m_WindowWidth = 1;
            m_WindowHeight = 1;
        }

        virtual glm::mat4 GetProjectionMatrix() const override
        {
            return glm::ortho(m_Xmin, m_Xmax, m_Ymin, m_Ymax, m_Zmin, m_Zmax);
        }

        virtual void SetWindowSize(int width, int height) override
        {
            m_WindowWidth = width;
            m_WindowHeight = height;

            // update the aspect ratio.
            // ...
        }

        virtual Ray GetCastingRay(int xpos, int ypos) const override
        {
            float x = (xpos / m_WindowWidth) * (m_Xmax - m_Xmin) + m_Xmin;
            float y = ((m_WindowHeight - ypos) / m_WindowHeight) * (m_Ymax - m_Ymin) + m_Ymin;
            glm::vec3 pt1(x, y, 0.0f);
            glm::vec3 pt2 = glm::vec3(x, y, -1.0f);
            vortex::Ray ray(pt1, pt2);
            return ray;
        }
    };
}
