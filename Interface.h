//
// Created by alina on 09.04.19.
//

#ifndef MAIN_INTERFACE_H
#define MAIN_INTERFACE_H

#include "ShaderProgram.h"
#include "glm/glm.hpp"
#include "constants.h"
#include "Utility.h"

using glm::mat4, glm::vec3, glm::vec2;


struct Interface {
    int height, width;
    double cursor[2] = {0, 0};
    double clicked[2] = {-1., -1.};
    double angles[2] = {0, 0};
    bool click = false;
    const ShaderProgram& aim_shader;
    const ShaderProgram& text_shader;
    Interface(int w, int h, const ShaderProgram& aim, const ShaderProgram& text, std::unordered_map<std::string, std::string> text_textures);
    vec3 click_dir();
    //void Draw();
    void Draw(int left, int right);
    void Draw(const std::string&);//win or loss
    void add_crack(vec2);
private:
    double far_size = NEAREST*glm::tan(FOV/2);
    float aim_pos[2] = {0, 0};
    unsigned int aim_VAO, aim_VBO, text_VAO, text_VBO, text_EBO;
    std::vector<std::pair<glm::vec2, mat4>> cracks;
    std::unordered_map<std::string, unsigned int> text_textures;
    void draw_aim();
    void draw_scores(int, int);
};

#endif //MAIN_INTERFACE_H
