#include "Texture.h"
#include <SOIL/SOIL.h>
#include <fstream>
#include <iostream>
Texture::Texture(const std::string &path_to_texture) {
    glGenTextures(1, &texture_id);
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    std::ifstream texture_file(path_to_texture);
    if (!texture_file.is_open()) {
        throw std::logic_error("Impossible to open: " + path_to_texture);
    }
    unsigned char * image = SOIL_load_image(path_to_texture.c_str(),
                                           &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    unbind();
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
