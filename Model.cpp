//
// Created by alina on 30.03.19.
//

#include "Model.h"

using glm::vec3, glm::vec2;


Model::Model(const std::string& path, mat4 t
) : transform(t) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::Draw() const {
    for (const auto& x: meshes) {
        x.Draw();
    }
}

void Model::Init() {
    for (auto &x : meshes) {
        x.Init();
    }
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Texture texture;

    vertices.resize(mesh->mNumVertices);
    for(int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.pos = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.tex_uv = (mesh->mTextureCoords[0])? vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : vec2(0.0f, 0.0f);
        vertices[i] = vertex;
    }

    for(int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        texture = loadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, "texture_diffuse");
    }
    return Mesh(vertices, indices, texture);
}


Texture Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    Texture texture;
    aiString str;
    mat->GetTexture(type, 0, &str);
    texture.id = TextureFromFile(std::string(str.C_Str()), directory);
    texture.type = typeName;
    texture.path = str;
    return texture;
}

