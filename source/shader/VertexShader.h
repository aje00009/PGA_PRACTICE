//
// Created by alber on 05/10/2025.
//

#ifndef PGA_PRACTICA_VERTEXSHADER_H
#define PGA_PRACTICA_VERTEXSHADER_H

#include "Shader.h"
/**
 * @class VertexShader Class that implements Shader class and manages the creating and compilation of the vertex shader
 */
namespace PAG {
    class VertexShader: public Shader {
        public:
            VertexShader(const std::string& filePath) : Shader(filePath, GL_VERTEX_SHADER) {}
    };
}

#endif //PGA_PRACTICA_VERTEXSHADER_H