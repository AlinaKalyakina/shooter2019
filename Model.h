//
// Created by alina on 30.03.19.
//

#ifndef MAIN_MODEL_H
#define MAIN_MODEL_H

#include <vector>
#include <string>
#include "Utility.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glm/glm.hpp"
using glm::mat4;

class Model {
public:
    Model(const std::string&, mat4 trans = mat4(1.f));
    void Init();
    void Draw() const;
    mat4 transform;
private:
    //const ShaderProgram& shader;
    std::vector<Mesh> meshes;
    std::string directory;
    //std::vector<Texture> textures_loaded;

    void processNode(aiNode*node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    Texture loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};


#endif //MAIN_MODEL_H
