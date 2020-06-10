//
// Created by alina on 07.04.19.
//

#ifndef MAIN_FIRE_H
#define MAIN_FIRE_H

#include "glm/glm.hpp"
#include "ShaderProgram.h"
#include "Model.h"

#include <vector>
#include <string>

class Fire {
    const ShaderProgram& shader;
    Model* model;
    float life = 0.f;
    float shift;
    mat4 model_mat = mat4(1.0);
public:
    bool Update(float delta);
    void Draw(mat4 view, mat4 projection);
    Fire(const ShaderProgram& shader, Model* model, vec3 pos);
};



#endif //MAIN_FIRE_H
