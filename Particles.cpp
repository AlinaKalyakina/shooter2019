//
// Created by alina on 07.04.19.
//

#include "Particles.h"
using glm::mat4;

#define MAX_DIST 40.0

Particles::Particles(const ShaderProgram &s): shader(s),
        x_y_dis((float)-MAX_DIST, (float)MAX_DIST), gen(rd())
{
    initialize_dots();
    glGenVertexArrays(1, &VAO); GL_CHECK_ERRORS;
    glGenBuffers(1, &VBO);GL_CHECK_ERRORS;

    glBindVertexArray(VAO);GL_CHECK_ERRORS;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, sizeof(dots), dots, GL_STATIC_DRAW);GL_CHECK_ERRORS;
    glEnableVertexAttribArray(0);GL_CHECK_ERRORS;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(dots[0]), (void*)0);GL_CHECK_ERRORS;

}

void Particles::Update(double delta) {
    for (auto& dot : dots) {
        if (dot.z > 2.) {
            dot.z = (float)-MAX_DIST;
            dot.x = x_y_dis(gen);
            dot.y = x_y_dis(gen);
        }
        dot.z += delta*PARTICLES_SPEED;
    }
}

void Particles::initialize_dots() {
    std::uniform_real_distribution<> z_dis(-MAX_DIST, 0.0);
    for (auto & dot: dots) {
        dot = vec3(x_y_dis(gen), x_y_dis(gen), z_dis(gen));
    }
}

void Particles::Draw(mat4 view, mat4 projection) {
    shader.StartUseShader();
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    shader.SetUniform("view", view);
    shader.SetUniform("projection", projection);
    glBindVertexArray(VAO); GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);GL_CHECK_ERRORS;

    void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);GL_CHECK_ERRORS;
    memcpy(ptr, dots, sizeof(dots));
    glUnmapBuffer(GL_ARRAY_BUFFER);GL_CHECK_ERRORS;

    glDrawArrays(GL_POINTS, 0, PARTICLES_NUM);GL_CHECK_ERRORS;
    glBindVertexArray(0);GL_CHECK_ERRORS;
    // always good practice to set everything back to defaults once configured.
    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
    shader.StopUseShader();
}
