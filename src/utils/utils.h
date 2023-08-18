/*
* Utility functions
*/
#pragma once
#include <string>


namespace vortex
{
    bool FileExists(const std::string& filePath);
    bool DirExists(const std::string& filePath);

    std::string LoadText(const char* file_path);

    std::string GetResourcePath(const std::string& relativePath);
}
