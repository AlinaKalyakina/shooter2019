//
// Created by alina on 30.03.19.
//


#include "Utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Mesh::Mesh(const std::vector<Vertex> &v, const std::vector<unsigned int> &ind, const Texture &t) :
        vertices(v), indices(ind), tex(t)
{
}

void Mesh::Init() {
    glGenVertexArrays(1, &VAO); GL_CHECK_ERRORS;
    glGenBuffers(1, &VBO);GL_CHECK_ERRORS;
    glGenBuffers(1, &EBO);GL_CHECK_ERRORS;

    glBindVertexArray(VAO);GL_CHECK_ERRORS;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);GL_CHECK_ERRORS;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);GL_CHECK_ERRORS;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);GL_CHECK_ERRORS;

    glEnableVertexAttribArray(0);GL_CHECK_ERRORS;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));GL_CHECK_ERRORS;

    glEnableVertexAttribArray(1);GL_CHECK_ERRORS;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_uv));GL_CHECK_ERRORS;
}


void Mesh::Draw() const {

    if (tex.id != 0)
        glBindTexture(GL_TEXTURE_2D, tex.id); GL_CHECK_ERRORS;
    // draw mesh
    glBindVertexArray(VAO); GL_CHECK_ERRORS;
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);GL_CHECK_ERRORS;
    glBindVertexArray(0);GL_CHECK_ERRORS

    // always good practice to set everything back to defaults once configured.
    //glActiveTexture(GL_TEXTURE0);GL_CHECK_ERRORS;
}

unsigned int TextureFromFile(std::string filename, const std::string &directory)
{
    if (filename == "")
        return 0;
    std::replace(filename.begin(), filename.end(),'\\', '/');

    auto n = filename.find_last_of('/');

    filename = filename.substr((n>=filename.length())?0:filename.find_last_of('/'), filename.length());//    directory = path.substr(0, path.find_last_of('/'));

    filename = directory + "/textures/" + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Bad texture channels";
            return 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);GL_CHECK_ERRORS;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);GL_CHECK_ERRORS;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);GL_CHECK_ERRORS;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);GL_CHECK_ERRORS;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);GL_CHECK_ERRORS;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);GL_CHECK_ERRORS;
        glGenerateMipmap(GL_TEXTURE_2D);GL_CHECK_ERRORS;
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int CubemapFromFile(std::string * paths) {
    unsigned int cubemap;
    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    int width, height, nrChannels;
    unsigned char* image;
    for (GLuint i = 0; i < 6; i++) {
        image = stbi_load(("../" + paths[i]).c_str(), &width, &height, &nrChannels, 0);
        if (!image) {
            std::cout << "Cubemap failed to load at path: " << paths[i] << std::endl;
            stbi_image_free(image);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
    //glGenerateTextureMipmap(cubemap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);GL_CHECK_ERRORS;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
