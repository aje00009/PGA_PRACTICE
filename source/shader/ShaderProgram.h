#ifndef PGA_PRACTICA_SHADERPROGRAM_H
#define PGA_PRACTICA_SHADERPROGRAM_H

#include <map>
#include <glm/glm.hpp>

#include "FragmentShader.h"
#include "VertexShader.h"

/**
 * @class ShaderProgram Class that implements the logic of creation of the shader program, compilation and linking of shaders
 */
namespace PAG {
    class ShaderProgram {
    private:
        VertexShader *vs = nullptr; ///< Vertex shader linked to this shader program
        FragmentShader *fs = nullptr; ///< Fragment shader linked to this shader program
        GLuint _programId = 0; ///< ID of shader program

        std::map<GLint, GLuint> _subroutineState; ///< Map saving ids of the subroutines currently chosen
        GLint _numActiveSubroutineUniforms; ///< Number of active subroutine uniforms
        std::map<std::string, GLint> _subroutineUniformLocations; ///< Map saving a pair nameUniform-IdUniform

    public:
        ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        ~ShaderProgram();

        void use() const;
        void setUniformMat4(const std::string& uniformName, const glm::mat4& matrix) const;
        void setUniformVec3(const std::string& uniformName, const glm::vec3& vector) const;
        void setUniformFloat(const std::string& uniformName, float value) const;
        GLuint getId() const;

        void queryStoreSubroutineInfo();
        GLuint getSubroutineIndex(const std::string& name) const;
        void activateSubroutine(GLuint subroutineIndex,const std::string& uniformName);
        void applySubroutines();
    };
}

#endif //PGA_PRACTICA_SHADERPROGRAM_H