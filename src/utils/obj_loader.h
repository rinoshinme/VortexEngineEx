#pragma once

#include <string>
#include <vector>

namespace vortex
{
    bool LoadObjFile(
        const std::string& filePath, 
        std::vector<float>& points, 
        std::vector<float>& texCoords,
        std::vector<float>& normals
    );
}
