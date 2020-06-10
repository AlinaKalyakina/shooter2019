//
// Created by alina on 07.04.19.
//

#include "Fire.h"
#include <random>
#include "constants.h"

Fire::Fire(const ShaderProgram& s, Model* m, vec3 pos) :shader(s),
            model(m), shift(rand() % 90), model_mat(glm::translate(mat4(1.f), pos)) {
}

bool Fire::Update(float delta) {
    life += delta;
    return life < EXPLODE_DURALITY;
}

void Fire::Draw(mat4 view, mat4 projection) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    shader.StartUseShader();
    shader.SetUniform("projection", projection);
    shader.SetUniform("view", view);
    shader.SetUniform("model", model_mat*model->transform*glm::rotate(mat4(1.0), glm::radians(40*life + shift), vec3(0., 1., 0.)));
    shader.SetUniform("time", life);
    model->Draw();
    shader.StopUseShader();
    glDisable(GL_BLEND);
}
