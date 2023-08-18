#include "utils.h"
#include <fstream>
#include <vector>


namespace vortex
{
    std::string LoadText(const char* file_path)
    {
        std::ifstream file;
        file.open(file_path, std::ios::ate);
        if (!file.is_open()) return "";

        size_t file_size = file.tellg();
        std::vector<char> buffer(file_size + 1);
        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), file_size);
        std::string result(buffer.data());
        return result;
    }

    std::string GetResourcePath(const std::string& relativePath)
    {
        return "F:/VortexLibs/VortexEngine/resources/" + relativePath;
    }


}
