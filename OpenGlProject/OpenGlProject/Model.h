#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

#include <algorithm>
#include <limits>
#include <cmath>
#include "Mesh.h"

class Model
{
public:
    Model(std::string path, unsigned int id);
    void Draw(
        Shader& shader, Camera& camera,
        glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
    unsigned int id;

private:
    std::vector<glm::mat4> matricesMeshes;

    std::string directory;

    std::vector<std::string> loadedTexName;
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};

#endif