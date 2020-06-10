//
// Created by alina on 05.04.19.
//

#ifndef MAIN_CUBEMAP_H
#define MAIN_CUBEMAP_H

#include "Utility.h"

class Cubemap {
    Texture texture;
    ShaderProgram& shader;
    unsigned int VAO, VBO;


public:
    Cubemap(std::string *, ShaderProgram&);
    void Draw(mat4, mat4);
};


#endif //MAIN_CUBEMAP_H
