//
// Created by alber on 06/10/2025.
//

#ifndef PGA_PRACTICA_SHADERPROGRAM_H
#define PGA_PRACTICA_SHADERPROGRAM_H

#include "glad/glad.h"
#include <string>

namespace PAG {
    class ShaderProgram {
    private:
        GLuint _programId = 0;

    public:
        ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        ~ShaderProgram(); // Usamos RAII

        void use() const;
        GLuint getId() const;
    };
}

#endif //PGA_PRACTICA_SHADERPROGRAM_H