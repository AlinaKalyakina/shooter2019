//
// Created by alina on 30.03.19.
//

#ifndef MAIN_UTILITY_H
#define MAIN_UTILITY_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <string>
#include <vector>
#include "ShaderProgram.h"
#include <assimp/Importer.hpp>

using glm::mat4, glm::mat3, glm::vec3;

struct Vertex
{
    glm::vec3 pos;
//    glm::vec3 n;
    glm::vec2 tex_uv;
//    glm::vec3 tg;
//    glm::vec3 bi_tg;
};

struct Texture {
    uint id;
    std::string type;
    aiString path;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex> &v, const std::vector<unsigned int> &, const Texture&);
    void Draw() const;
    void Init();

private:
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    const Texture tex;
};

unsigned int TextureFromFile(std::string path, const std::string &directory);
unsigned int CubemapFromFile(std::string* paths);

#endif //MAIN_UTILITY_H
