#ifndef PGA_PRACTICA_SHADERPROGRAM_H
#define PGA_PRACTICA_SHADERPROGRAM_H

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
        void setUniformMat4(const std::string& uniformName, const glm::mat4& matrix) const;
        void setUniformVec3(const std::string& uniformName, const glm::vec3& vector) const;
        void setUniformFloat(const std::string& uniformName, float value) const;
        GLuint getId() const;

        GLuint getSubroutineIndex(const std::string& name) const;
        void activateSubroutine(GLuint subroutineIndex) const;
    };
}

#endif //PGA_PRACTICA_SHADERPROGRAM_H