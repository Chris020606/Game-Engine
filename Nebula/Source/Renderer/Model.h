//
// Created by Christian Cruz on 3/18/26.
//

#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "Core.h"
#include "Mesh.h"
#include "Texture.h"
#include <assimp/material.h>
#include <assimp/scene.h>

// struct aiNode;
// struct aiScene;
// struct aiMesh;
// struct aiMaterial;
// enum aiTextureType;


namespace Nebula {

    class Model {
        public:
            static Ref<Model> Create(std::string const &path, bool gamma = false);

            const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }
            const std::string& GetDirectory() const { return m_Directory; }

        private:
            Model() = default;
            std::vector<Ref<Texture2D>> m_TexturesLoaded;
            std::vector<Ref<Mesh>> m_Meshes;
            std::string m_Directory;
            bool gammaCorrection;

            void loadModel(std::string const &path);
            void processNode(aiNode *node, const aiScene *scene);

            Ref<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);

            std::vector<Ref<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

    };

    class ModelLoader {
    public:
        static Model Load(const std::string& path);
    };

    class ModelAnimation {

    };

} // Nebula

#endif //MODEL_H
