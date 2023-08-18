#pragma once

#include <glm/glm.hpp>

namespace vortex
{
    struct Sphere
    {
        glm::vec3 center;
        float radius;

        Sphere(const glm::vec3& c, float r)
            : center(c), radius(r) {}
    };

    struct AABB
    {
        glm::vec3 minCoords;
        glm::vec3 maxCoords;

        AABB(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
        {
            // 
        }
    };
    
}
