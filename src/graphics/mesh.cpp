#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h> // various extra operations
#include <assimp/scene.h>       // collects data



namespace vortex
{
    bool Mesh::LoadFromFile(const std::string& filePath)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene)
            return false;

        printf("  %i animations\n", scene->mNumAnimations);
        printf("  %i cameras\n", scene->mNumCameras);
        printf("  %i lights\n", scene->mNumLights);
        printf("  %i materials\n", scene->mNumMaterials);
        printf("  %i meshes\n", scene->mNumMeshes);
        printf("  %i textures\n", scene->mNumTextures);

        if (scene->mNumMeshes == 0)
            return false;

        // get first mesh
        const aiMesh* mesh = scene->mMeshes[0];
        int numVertices = mesh->mNumVertices;
        for (int i = 0; i < numVertices; ++i)
        {
            Vertex v;
            auto vp = mesh->mVertices[i];
            auto vt = mesh->mTextureCoords[0][i];
            auto vn = mesh->mNormals[i];
            v.Position.x = vp.x;
            v.Position.y = vp.y;
            v.Position.z = vp.z;
            v.TexCoords.x = vt.x;
            v.TexCoords.y = vt.y;
            v.Normal.x = vn.x;
            v.Normal.y = vn.y;
            v.Normal.z = vn.z;

            m_Vertices.push_back(v);
        }
        return true;
    }

    const float* Mesh::DataPtr() const
    {
        return &m_Vertices[0].Position.x;
    }

}
