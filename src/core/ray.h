#pragma once

#include <glm/glm.hpp>
#include "simple_geometry_objects.h"

namespace vortex
{
    struct Ray
    {
        glm::vec3 center;
        glm::vec3 direction;

        Ray(const glm::vec3& c, const glm::vec3 dir);
        static Ray FromPoints(const glm::vec3& pt1, const glm::vec3& pt2);
        void Transform(const glm::mat4& mat);

        float Intersect(const Sphere& s);
    };
    
}
