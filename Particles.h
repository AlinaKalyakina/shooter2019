//
// Created by alina on 07.04.19.
//

#ifndef MAIN_PARTICTES_H
#define MAIN_PARTICTES_H
#include "glm/glm.hpp"
#include "ShaderProgram.h"
#include <random>
#include "constants.h"

using glm::vec3, glm::mat4;

class Particles {
    glm::vec3 dots[PARTICLES_NUM];
    const ShaderProgram& shader;
    //double speed;
    unsigned int VAO, VBO;
    std::uniform_real_distribution<float> x_y_dis;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen;
    void initialize_dots();
public:
    Particles(const ShaderProgram& shader);
    void Update(double delta);
    void Draw(mat4 view, mat4 projection);
};


#endif //MAIN_PARTICTES_H
