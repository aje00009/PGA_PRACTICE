#ifndef PGA_PRACTICA_SHADERPROGRAM_H
#define PGA_PRACTICA_SHADERPROGRAM_H

#include <map>
#include <string> // Asegúrate de incluir string
#include <glm/glm.hpp>
#include <glad/glad.h> // Necesario para GLuint/GLint

#include "FragmentShader.h"
#include "VertexShader.h"

namespace PAG {
    class ShaderProgram {
    private:
        VertexShader *vs = nullptr;
        FragmentShader *fs = nullptr;
        GLuint _programId = 0;

        std::map<GLint, GLuint> _subroutineState;
        GLint _numActiveSubroutineUniforms = 0;
        std::map<std::string, GLint> _subroutineUniformLocations;
        std::map<std::string, GLuint> _subroutineIndices;

    public:
        ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        ~ShaderProgram();

        void use() const;
        void setUniformMat4(const std::string& uniformName, const glm::mat4& matrix) const;
        void setUniformVec3(const std::string& uniformName, const glm::vec3& vector) const;
        void setUniformMat3(const std::string& uniformName, const glm::mat3& matrix) const;
        void setUniformFloat(const std::string& uniformName, float value) const;
        void setUniformInt(const std::string& uniformName, int value) const;

        GLuint getId() const;

        void queryStoreSubroutineInfo();

        GLuint getSubroutineIndex(const std::string& name) const;

        void activateSubroutine(const std::string& implementationName, const std::string& uniformName);

        void applySubroutines();
    };
}

#endif //PGA_PRACTICA_SHADERPROGRAM_H