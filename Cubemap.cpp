//
// Created by alina on 05.04.19.
//

#include "Cubemap.h"

static const float skybox_info[36*3] = {
// positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
};

Cubemap::Cubemap(std::string* paths, ShaderProgram& s) : shader(s)
{
    texture.id = CubemapFromFile(paths);
    texture.path = paths[0];
    texture.type = "cubemap";

    glGenVertexArrays(1, &VAO); GL_CHECK_ERRORS;
    glGenBuffers(1, &VBO); GL_CHECK_ERRORS;
    glBindVertexArray(VAO); GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, VBO); GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_info), &skybox_info, GL_STATIC_DRAW); GL_CHECK_ERRORS;
    glEnableVertexAttribArray(0); GL_CHECK_ERRORS;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); GL_CHECK_ERRORS;

}

void Cubemap::Draw(mat4 view, mat4 projection) {
    shader.StartUseShader(); GL_CHECK_ERRORS;
    shader.SetUniform("view", mat4(mat3(view))); GL_CHECK_ERRORS;
    shader.SetUniform("projection", projection); GL_CHECK_ERRORS;
    glDepthMask(GL_FALSE); GL_CHECK_ERRORS;
    glBindVertexArray(VAO); GL_CHECK_ERRORS;
    glDrawArrays(GL_TRIANGLES, 0, 36); GL_CHECK_ERRORS;
    glBindVertexArray(0);GL_CHECK_ERRORS;
    glDepthMask(GL_TRUE);GL_CHECK_ERRORS;
}