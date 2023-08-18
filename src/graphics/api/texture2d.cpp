#include "texture2d.h"

namespace vortex
{
    Texture2D::Texture2D()
    {
        glGenTextures(1, &m_TextureID);
    }

    Texture2D::~Texture2D()
    {
        if (m_TextureID != 0)
            glDeleteTextures(1, &m_TextureID);
    }

    void Texture2D::Bind(int slot) const
    {
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glActiveTexture(GL_TEXTURE0 + slot);
    }

    void Texture2D::Unbind() const 
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    std::shared_ptr<Texture2D> Texture2D::FromFile(const std::string& filePath)
    {
        std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
        texture->LoadFile(filePath);
        return texture;
    }

    void Texture2D::LoadFile(const std::string& filePath)
    {
        int x, y, n;
        int force_channels = 4;
        // the following function call flips the image
        // needs to be called before each stbi_load(...);
        stbi_set_flip_vertically_on_load(true);
        unsigned char* image_data = stbi_load(filePath.c_str(), &x, &y, &n, force_channels);
        if ( !image_data ) {
          fprintf( stderr, "ERROR: could not load %s\n", filePath.c_str() );
          return;
        }
        // NPOT check
        if ( ( x & ( x - 1 ) ) != 0 || ( y & ( y - 1 ) ) != 0 ) { fprintf( stderr, "WARNING: texture %s is not power-of-2 dimensions\n", filePath.c_str() ); }

        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        GLfloat max_aniso = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
        // set the maximum!
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
    }


}
