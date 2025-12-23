#include "Texture.h"

#include <cstring>

#include "lodepng.h"

#include <vector>
#include <stdexcept>
#include <iostream>

/**
 * @brief Parameterized constructor of class Texture
 * @param path Path of the texture (.png)
 * @post This constructor will load a texture given by its path (fliping it before), configure and bind it in OpenGL
 */
PAG::Texture::Texture(const std::string& path) : _path(path) {
    std::vector<unsigned char> image;
    unsigned width, height;

    // 1. Load PNG
    unsigned error = lodepng::decode(image, width, height, path);
    if (error) {
        throw std::runtime_error("Error loading texture " + path + ": " + lodepng_error_text(error));
    }

    // 2. Flip image
    std::vector<unsigned char> imageFlipped(image.size());
    unsigned int stride = width * 4; 
    for (unsigned int y = 0; y < height; y++) {
        unsigned char* srcRow = image.data() + (y * stride);
        unsigned char* dstRow = imageFlipped.data() + ((height - 1 - y) * stride);
        std::memcpy(dstRow, srcRow, stride);
    }

    _width = width;
    _height = height;

    // 3. Generate texture in OpenGL
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    // Texture configuration
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Send data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageFlipped.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

PAG::Texture::~Texture() {
    if (_id != 0) glDeleteTextures(1, &_id);
}

/**
 * @brief Method that binds a texture given by its id to the current texture managed by OpenGL
 */
void PAG::Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, _id);
}