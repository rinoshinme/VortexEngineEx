#include "ray.h"
#include <cmath>

namespace vortex
{
    Ray::Ray(const glm::vec3& c, const glm::vec3 dir)
    {
        center = c;
        direction = glm::normalize(dir);
    }

    Ray Ray::FromPoints(const glm::vec3& pt1, const glm::vec3& pt2)
    {
        glm::vec3 dir = pt2 - pt1;
        return Ray(pt1, dir);
    }

    void Ray::Transform(const glm::mat4& mat)
    {
        glm::vec3 pt2 = center + direction;
        center = glm::vec3(mat * glm::vec4(center, 1.0));
        glm::vec3 t2 = glm::vec3(mat * glm::vec4(pt2, 1.0));
        direction = glm::normalize(t2 - center);
    }

    float Ray::Intersect(const Sphere& s)
    {
        float a = glm::dot(direction, direction);
        glm::vec3 off = center - s.center;
        float b = 2 * glm::dot(direction, off);
        float c = glm::dot(off, off) - s.radius * s.radius;
        float delta = b * b - 4 * a * c;
        if (delta < 0)
            return -1;
        // check if the smaller one is in front of the camera
        float v1 = (-b - sqrt(delta)) / (2 * a);
        if (v1 >= 0)
            return v1;
        // check the further one
        float v2 =  (-b + sqrt(delta)) / (2 * a);
        if (v2 < 0)
            return -1;
        return v2;
    }
}
