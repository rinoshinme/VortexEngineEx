#include "skybox.h"
#include "stb_image.h"

namespace vortex
{
    SkyBox::SkyBox(const std::vector<std::string>& filePaths, int textureSlot, 
        const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        m_TextureSlot = textureSlot;
        MakeVAO();
        MakeTexture(filePaths);

        std::vector<vortex::ShaderSource> sources = {
            {
                vortex::ShaderType::SHADER_TYPE_VERTEX, vertexShaderPath
            },
            {
                vortex::ShaderType::SHADER_TYPE_PIXEL, fragmentShaderPath
            }
        };

        m_Shader = vortex::Shader::FromSources(sources);
    }

    SkyBox::~SkyBox()
    {
        // 
    }

    void SkyBox::Draw(const glm::mat4& view, const glm::mat4& proj)
    {
        glDepthMask(GL_FALSE);
        m_Shader->Bind();
        m_Shader->SetUniformMatrix4("V", view);
        m_Shader->SetUniformMatrix4("P", proj);
        m_Shader->SetUniformInt("cube_texture", m_TextureSlot);
        glActiveTexture(GL_TEXTURE0 + m_TextureSlot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
    }

    void SkyBox::MakeVAO()
    {
        float points[] = { 
            -10.0f, 10.0f, -10.0f, -10.0f, -10.0f, -10.0f, 10.0f, -10.0f, -10.0f, 10.0f, -10.0f, -10.0f, 10.0f, 10.0f, -10.0f, -10.0f, 10.0f, -10.0f,
            -10.0f, -10.0f, 10.0f, -10.0f, -10.0f, -10.0f, -10.0f, 10.0f, -10.0f, -10.0f, 10.0f, -10.0f, -10.0f, 10.0f, 10.0f, -10.0f, -10.0f, 10.0f,
            10.0f, -10.0f, -10.0f, 10.0f, -10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, -10.0f, 10.0f, -10.0f, -10.0f,
            -10.0f, -10.0f, 10.0f, -10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, -10.0f, 10.0f, -10.0f, -10.0f, 10.0f,
            -10.0f, 10.0f, -10.0f, 10.0f, 10.0f, -10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, -10.0f, 10.0f, 10.0f, -10.0f, 10.0f, -10.0f,
            -10.0f, -10.0f, -10.0f, -10.0f, -10.0f, 10.0f, 10.0f, -10.0f, -10.0f, 10.0f, -10.0f, -10.0f, -10.0f, -10.0f, 10.0f, 10.0f, -10.0f, 10.0f };
        
        glGenBuffers( 1, &m_VBO );
        glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
        glBufferData( GL_ARRAY_BUFFER, 3 * 36 * sizeof( GLfloat ), &points, GL_STATIC_DRAW );

        glGenVertexArrays( 1, &m_VAO );
        glBindVertexArray( m_VAO );
        glEnableVertexAttribArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    }

    void SkyBox::MakeTexture(const std::vector<std::string>& filePaths)
    {
        glActiveTexture(GL_TEXTURE0 + m_TextureSlot);
        glGenTextures(1, &m_TextureID);

        // load each image and copy into a side of the cube-map texture
        LoadCubemapSide(m_TextureID, filePaths[0], GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
        LoadCubemapSide(m_TextureID, filePaths[1], GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
        LoadCubemapSide(m_TextureID, filePaths[2], GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
        LoadCubemapSide(m_TextureID, filePaths[3], GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
        LoadCubemapSide(m_TextureID, filePaths[4], GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
        LoadCubemapSide(m_TextureID, filePaths[5], GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        // format cube map texture
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void SkyBox::LoadCubemapSide(GLuint texture, const std::string& filePath, GLenum side_target)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        stbi_set_flip_vertically_on_load(false);
        int x, y, n;
        int force_channels = 4;
        unsigned char* image_data = stbi_load(filePath.c_str(), &x, &y, &n, force_channels );
        if ( !image_data ) {
            fprintf( stderr, "ERROR: could not load %s\n", filePath.c_str() );
            return;
        }
        // non-power-of-2 dimensions check
        if ( ( x & ( x - 1 ) ) != 0 || ( y & ( y - 1 ) ) != 0 ) { fprintf( stderr, "WARNING: image %s is not power-of-2 dimensions\n", filePath.c_str() ); }

        // copy image data into 'target' side of cube map
        glTexImage2D( side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data );
        free( image_data );
    }

}
