#pragma once
#include <memory>
#include <string>
#include <GL/glew.h>
#include <stb_image.h>


namespace vortex
{
    class Texture2D
    {
    private:
        GLuint m_TextureID;

    public:
        static std::shared_ptr<Texture2D> FromFile(const std::string& filePath);

        Texture2D();
        ~Texture2D();
        void Bind(int slot = 0) const;
        void Unbind() const;

    private:
        void LoadFile(const std::string& filePath);
    };
}
