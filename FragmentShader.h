//
// Created by alber on 05/10/2025.
//

#ifndef PGA_PRACTICA_FRAGMENTSHADER_H
#define PGA_PRACTICA_FRAGMENTSHADER_H

#include "Shader.h"
namespace PAG {
    class FragmentShader : public Shader {
    public:
        FragmentShader(const std::string& filePath) : Shader(filePath, GL_FRAGMENT_SHADER) {}
    };
}

#endif //PGA_PRACTICA_FRAGMENTSHADER_H