#include "shader.h"
#include "utils/utils.h"
#include <iostream>

namespace vortex
{
    Shader::Shader()
    {
        m_ProgramID = glCreateProgram();
    }

    Shader::~Shader()
    {
        if (m_ProgramID != 0)
            glDeleteProgram(m_ProgramID);
    }

    std::shared_ptr<Shader> Shader::FromSources(const std::vector<ShaderSource>& shaderSources)
    {
        auto shader = std::make_shared<Shader>();
        shader->LoadShaderSources(shaderSources);
        return shader;
    }

    void Shader::LoadShaderSources(const std::vector<ShaderSource>& shaderSources)
    {
        for (auto& source : shaderSources)
        {
            const std::string source_code_str = vortex::LoadText(source.filepath.c_str());
            const char* source_code = source_code_str.c_str();
            GLuint shader = 0;
            if (source.type == ShaderType::SHADER_TYPE_VERTEX)
            {
                shader = glCreateShader(GL_VERTEX_SHADER);
            }
            else if (source.type == ShaderType::SHADER_TYPE_PIXEL)
            {
                shader = glCreateShader(GL_FRAGMENT_SHADER);
            }

            if (shader > 0)
            {
                glShaderSource(shader, 1, &source_code, NULL);
                glCompileShader(shader);
                // log
                GLint result;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
                if (result == GL_FALSE)
                {
                    GLint length;
                    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                    std::vector<char> error(length);
                    glGetShaderInfoLog(shader, length, &length, &error[0]);
                    std::cout << "Failed to compile vertex shader!" << std::endl <<
                        &error[0] << std::endl;
                    glDeleteShader(shader);
                    return;
                }

                glAttachShader(m_ProgramID, shader);
                glDeleteShader(shader);
            }
        }
        // link 
        glLinkProgram(m_ProgramID);
    }

    void Shader::Bind() const 
    {
        glUseProgram(m_ProgramID);
    }

    void Shader::UnBind() const 
    {
        glUseProgram(0);
    }

    void Shader::SetUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
        glUniform1i(location, value);
    }

    void Shader::SetUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
        glUniform1f(location, value);
    }

    void Shader::SetUniformMatrix4(const std::string& name, float* mat)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, mat);
    }

    void Shader::SetUniformMatrix4(const std::string& name, const glm::mat4& mat)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::SetUniformVec3(const std::string& name, float* values)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
        glUniform3fv(location, 1, values);
    }
}
