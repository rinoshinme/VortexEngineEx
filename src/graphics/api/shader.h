#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace vortex
{
    enum class ShaderType
    {
        SHADER_TYPE_VERTEX,
        SHADER_TYPE_PIXEL,
        SHADER_TYPE_GEOMETRY,
        SHADER_TYPE_COMPUTE,
    };

    struct ShaderSource
    {
        ShaderType type;
        std::string filepath;
    };

    class Shader
    {
    private:
        GLuint m_ProgramID;

    public:
        static std::shared_ptr<Shader> FromSources(const std::vector<ShaderSource>& shaderSources);

        Shader();
        ~Shader();
        void Bind() const;
        void UnBind() const;
        GLint Id() const { return m_ProgramID; }

        void SetUniformInt(const std::string& name, int value);
        void SetUniformFloat(const std::string& name, float value);
        void SetUniformVec3(const std::string& name, float* values);
        void SetUniformMatrix4(const std::string& name, float* mat);
        void SetUniformMatrix4(const std::string& name, const glm::mat4& mat);

    private:
        void LoadShaderSources(const std::vector<ShaderSource>& shaderSources);
    };
}
