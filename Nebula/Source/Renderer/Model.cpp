//
// Created by Christian Cruz on 3/18/26.
//

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"

namespace Nebula {

    Ref<Model> Model::Create(const std::string& path, bool gamma) {
        // Can't use make_shared with private constructor, so:
        Ref<Model> model(new Model());
        model->loadModel(path);
        return model;
    }

    void Model::loadModel(const std::string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_CalcTangentSpace
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        m_Directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Ref<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // --- Vertices ---
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex{};

            vertex.position = {
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            };

            if (mesh->HasNormals()) {
                vertex.normal = {
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                };
            }

            if (mesh->mTextureCoords[0]) {
                vertex.texCoords = {
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                };

                float u = mesh->mTextureCoords[0][i].x;
                float v = mesh->mTextureCoords[0][i].y;


                if (i < 5) printf("UV[%d] = (%.3f, %.3f)\n", i, u, v);

            } else {
                vertex.texCoords = { 0.0f, 0.0f };
            }

            // color defaults to white if mesh has no vertex colors
            if (mesh->HasVertexColors(0)) {

                vertex.color = {
                    mesh->mColors[0][i].r,
                    mesh->mColors[0][i].g,
                    mesh->mColors[0][i].b
                };

            } else {
                vertex.color = { 1.0f, 1.0f, 1.0f };
            }

            vertices.push_back(vertex);
        }

        // --- Indices ---
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // --- Textures (stored on model, passed to mesh later via material) ---
        // You'll hook this into your Texture abstraction when that's ready
        // For now mesh is created with just geometry
        return std::make_shared<Mesh>(vertices, indices);
    }

    std::vector<Ref<Texture2D>> Model::loadMaterialTextures(
        aiMaterial* mat, aiTextureType type, const std::string& typeName)
    {
        std::vector<Ref<Texture2D>> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string fullPath = m_Directory + "/" + str.C_Str();

            // Check cache
            bool skip = false;
            for (auto& loaded : m_TexturesLoaded) {
                if (loaded->GetPath() == fullPath) {
                    textures.push_back(loaded);
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                auto texture = Texture2D::Create(fullPath);
                textures.push_back(texture);
                m_TexturesLoaded.push_back(texture);
            }
        }

        return textures;
    }

} // Nebula