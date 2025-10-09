//
// Created by alber on 06/10/2025.
//

#ifndef PGA_PRACTICA_SHADERPROGRAM_H
#define PGA_PRACTICA_SHADERPROGRAM_H

#include "glad/glad.h"
#include <string>

#include "FragmentShader.h"
#include "VertexShader.h"

/**
 * @class ShaderProgram Class that implements the logic of creation of the shader program, compilation and linking of shaders
 */
namespace PAG {
    class ShaderProgram {
    private:
        VertexShader *vs = nullptr;
        FragmentShader *fs = nullptr;
        GLuint _programId = 0;

    public:
        ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        ~ShaderProgram(); // Usamos RAII

        void use() const;
        GLuint getId() const;
    };
}

#endif //PGA_PRACTICA_SHADERPROGRAM_H