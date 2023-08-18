/**
 * skybox use single CUBE MAP texture to store 6 side images.
 * 
 */
#pragma once

#include <string>
#include <vector>
#include <map>
#include <graphics/api.h>
#include <GL/glew.h>

namespace vortex
{
    class SkyBox
    {
    private:
        std::shared_ptr<Shader> m_Shader;
        GLuint m_TextureID;
        int m_TextureSlot;
        GLuint m_VAO;
        GLuint m_VBO;
        
    public:
        // file paths
        // front/back/top/bottom/left/right
        SkyBox(const std::vector<std::string>& filePaths, int textureSlot, 
            const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        ~SkyBox();

        void Draw(const glm::mat4& view, const glm::mat4& proj);

    private:
        void MakeVAO();
        void MakeTexture(const std::vector<std::string>& filePaths);
        void LoadCubemapSide(GLuint texture, const std::string& filePath, GLenum side_target);
    };
}
