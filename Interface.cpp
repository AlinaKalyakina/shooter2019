//
// Created by alina on 09.04.19.
//

#include "Interface.h"
#include "glm/ext.hpp"
#include "constants.h"
#include <string>
#define CRUTCH 0.98

using glm::vec3;

static const float text_boxes[] {
        1.f, 1.f, 1.0f, 0.0f, // top right
        1.f, -1.f, 1.0f, 1.0f, // bottom right
        -1.f, -1.f, 0.0f, 1.0f, // bottom left
        -1.f, 1.f, 0.0f, 0.0f // top left
};

static const unsigned int text_indices[] {
    0, 1, 3,
    1, 2, 3
};

Interface::Interface(int w, int h, const ShaderProgram &aim, const ShaderProgram& text, std::unordered_map<std::string, std::string> textures)
    : aim_shader(aim), text_shader(text), width(w), height(h){
    glGenBuffers(1, &aim_VBO);
    glGenVertexArrays(1, &aim_VAO);
    glBindVertexArray(aim_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, aim_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(aim_pos), &aim_pos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    glGenBuffers(1, &text_VBO);
    glGenVertexArrays(1, &text_VAO);
    glGenBuffers(1, &text_EBO);GL_CHECK_ERRORS;
    glBindVertexArray(text_VAO);

    glBindVertexArray(text_VAO);GL_CHECK_ERRORS;

    glBindBuffer(GL_ARRAY_BUFFER, text_VBO);GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, sizeof(text_boxes), text_boxes, GL_STATIC_DRAW);GL_CHECK_ERRORS;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, text_EBO);GL_CHECK_ERRORS;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(text_indices), text_indices, GL_STATIC_DRAW);GL_CHECK_ERRORS;

    glEnableVertexAttribArray(0);GL_CHECK_ERRORS;
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);GL_CHECK_ERRORS;

    glEnableVertexAttribArray(1);GL_CHECK_ERRORS;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));GL_CHECK_ERRORS;

    for (auto& x : textures) {
        text_textures[x.first] = TextureFromFile(x.second, "../");
    }

}

void Interface::draw_aim() {
    aim_pos[0] = 2 * cursor[0] / width - 1;
    aim_pos[1] = 1 - 2 * cursor[1] / height;
    aim_shader.StartUseShader();
    glDepthMask(GL_FALSE);    GL_CHECK_ERRORS;
    glBindVertexArray(aim_VAO);    GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, aim_VBO);    GL_CHECK_ERRORS;

    void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);    GL_CHECK_ERRORS;
    memcpy(ptr, aim_pos, sizeof(aim_pos));
    glUnmapBuffer(GL_ARRAY_BUFFER);
    GL_CHECK_ERRORS;

    aim_shader.SetUniform("g_screenWidth", width);
    aim_shader.SetUniform("g_screenHeight", height);
    aim_shader.SetUniform("color", glm::vec4(1., 1., 0., 1.));
    glDrawArrays(GL_POINTS, 0, 1);
    GL_CHECK_ERRORS;
    glDepthMask(GL_TRUE);
    GL_CHECK_ERRORS;
    aim_shader.StopUseShader();
}

vec3 Interface::click_dir() {
    if (!click) {
        return vec3(0., 0., 0.);
    }
    double tmp[2];
    tmp[0] = 2 * clicked[0] / width - 1;
    tmp[1] = 1 - 2 * clicked[1] / height;
    click = false;
    return normalize(vec3(tmp[0]*far_size*CRUTCH, CRUTCH*tmp[1]*far_size*height/width, -NEAREST));
}

void Interface::Draw(int life, int score) {
    draw_aim();
    draw_scores(life, score);
}

void Interface::draw_scores(int life, int score) {
    std::vector<std::pair<unsigned int,mat4>> text_transform;
//    auto basic_model = glm::scale(mat4(1.0), vec3(5*CHARACTER_SIZE, 5*CHARACTER_SIZE, 5*CHARACTER_SIZE));
//    for (const auto& pos : cracks) {
//        text_transform.emplace_back(std::pair<unsigned int, mat4>(text_textures["crack"],
//                glm::translate(mat4(1.0), vec3(pos.x, pos.y, 0))*basic_model));
//    }
    glDepthMask(GL_FALSE);    GL_CHECK_ERRORS;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    text_shader.StartUseShader();
        text_transform.emplace_back(std::pair<unsigned int, mat4>(text_textures["cabin"], mat4(1.0)));
        auto str = std::to_string(life);
        auto basic_model = glm::scale(mat4(1.0), vec3(CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE));
        vec3 begin = vec3(-1+CHARACTER_SIZE + IDENT, -1+CHARACTER_SIZE + IDENT, 0);
        std::string tmp = "0";
        for (int i = 0; i < str.size(); i++) {
            tmp[0] = str[i];
            text_transform.emplace_back((std::pair<unsigned int, mat4>(text_textures[tmp],
                    glm::translate(mat4(1.0), begin + vec3(2*CHARACTER_SIZE*i, 0, 0))*basic_model)));
        }
        str = std::to_string(score);
        begin = vec3(1+CHARACTER_SIZE-IDENT, -1+CHARACTER_SIZE+IDENT, 0) - float(str.size())*vec3(2*CHARACTER_SIZE, 0, 0);
        for (int i = 0; i < str.size(); i++) {
            tmp[0] = str[i];
            text_transform.emplace_back((std::pair<unsigned int, mat4>(text_textures[tmp],
                    glm::translate(mat4(1.0), begin + vec3(2*CHARACTER_SIZE*i, 0, 0))*basic_model)));
        }
    //}
    glBindVertexArray(text_VAO); GL_CHECK_ERRORS
    glBindTexture(GL_TEXTURE_2D, text_textures["crack"]);
    for (const auto& x: cracks) {
        text_shader.SetUniform("model", x.second);
        glDrawElements(GL_TRIANGLES, sizeof(text_indices), GL_UNSIGNED_INT, (void*)0);GL_CHECK_ERRORS;
    }
    for (auto& x:text_transform) {
        text_shader.SetUniform("model", x.second);
        glBindTexture(GL_TEXTURE_2D, x.first); GL_CHECK_ERRORS;
        glDrawElements(GL_TRIANGLES, sizeof(text_indices), GL_UNSIGNED_INT, (void*)0);GL_CHECK_ERRORS;
    }
    glBindVertexArray(0);GL_CHECK_ERRORS
    text_shader.StopUseShader();
    glDepthMask(GL_TRUE);    GL_CHECK_ERRORS;
    glDisable(GL_BLEND);
}

void Interface::Draw(const std::string& state) {
    glDepthMask(GL_FALSE);
    text_shader.StartUseShader();
    text_shader.SetUniform("model", mat4(1.0));
    glBindVertexArray(text_VAO); GL_CHECK_ERRORS;
    glBindTexture(GL_TEXTURE_2D, text_textures[state]);
    glDrawElements(GL_TRIANGLES, sizeof(text_indices), GL_UNSIGNED_INT, (void*)0);GL_CHECK_ERRORS;
    glBindVertexArray(0);GL_CHECK_ERRORS
    text_shader.StopUseShader();
    glDepthMask(GL_TRUE);    GL_CHECK_ERRORS;
}

void Interface::add_crack(vec2 pos) {
    pos = 2.f*pos;
    for (const auto& x : cracks){
        if (glm::length(x.first-pos) < 0.2) {
            return;
        }
    }
    auto mat = glm::translate(mat4(1.0), vec3(pos.x, pos.y, 0))*
            glm::rotate(mat4(1.0), float(rand() % 10)/2, vec3(0, 0, 1))*
            glm::scale(mat4(1.0), vec3(5*CHARACTER_SIZE, 5*CHARACTER_SIZE, 5*CHARACTER_SIZE));
    cracks.push_back(std::pair<glm::vec2, mat4>(pos, mat));
}