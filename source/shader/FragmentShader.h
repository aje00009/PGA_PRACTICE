#ifndef PGA_PRACTICA_FRAGMENTSHADER_H
#define PGA_PRACTICA_FRAGMENTSHADER_H

#include "Shader.h"

/**
 * @class FragmentShader Class that implements Shader class and manages the creating and compilation of the fragment shader
 */
namespace PAG {
    class FragmentShader : public Shader {
    public:
        FragmentShader(const std::string& filePath) : Shader(filePath, GL_FRAGMENT_SHADER) {}
    };
}

#endif //PGA_PRACTICA_FRAGMENTSHADER_H