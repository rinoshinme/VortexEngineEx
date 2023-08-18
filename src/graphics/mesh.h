#pragma once

#include <vector>
#include <string>
#include "vertex.h"

namespace vortex
{
    class Mesh
    {
    private:
        std::vector<Vertex> m_Vertices;
    public:
        Mesh() {}

        bool LoadFromFile(const std::string& filePath);

        const float* DataPtr() const;
        uint32_t NumVertices() const { return m_Vertices.size(); }
    };
}
