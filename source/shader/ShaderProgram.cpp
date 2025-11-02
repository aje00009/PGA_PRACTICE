#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "VertexShader.h"
#include "FragmentShader.h"

/**
 * @brief Default constructor of ShaderProgram
 * @param vertexShaderPath Relative path of the vertex shader file we want to upload
 * @param fragmentShaderPath Relative path of the fragment shader file we want to upload
 * @post The constructor will create the shader program and link both vertex and fragment shader to it
 */
PAG::ShaderProgram::ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    _programId = glCreateProgram();
    if (_programId == 0) {
        throw std::runtime_error("Error creating shader program.");
    }

    vs = new VertexShader(vertexShaderPath);
    fs = new FragmentShader(fragmentShaderPath);

    glAttachShader(_programId, vs->getId());
    glAttachShader(_programId, fs->getId());
    glLinkProgram(_programId);

    // Check for linking errors
    GLint success;
    glGetProgramiv(_programId, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            std::string infoLog(logLength, ' ');
            glGetProgramInfoLog(_programId, logLength, nullptr, &infoLog[0]);
            glDeleteProgram(_programId); // Don't leak the program.
            _programId = 0;
            throw std::runtime_error("Shader program linking failed:\n" + infoLog);
        }
    }
}

/**
 * @brief Destructor of the class ShaderProgram
 */
PAG::ShaderProgram::~ShaderProgram() {
    if (_programId != 0) {
        glDeleteProgram(_programId);
    }
    if (vs->getId() != 0)
        glDeleteShader(vs->getId());
    if (fs->getId() != 0)
        glDeleteShader(fs->getId());
}

/**
 * @brief Method that tells OpenGL to use the program id of this object
 */
void PAG::ShaderProgram::use() const {
    glUseProgram(_programId);
}

void PAG::ShaderProgram::setUniformMat4(const std::string &uniformName, const glm::mat4 &matrix) {
    GLint position = glGetUniformLocation(_programId, uniformName.c_str());

    if ( position != -1 )
        glUniformMatrix4fv(position, 1, GL_FALSE, glm::value_ptr(matrix));
}

/**
 * @brief Getter that returns the id of the shader program
 * @return The id of the shader program
 */
GLuint PAG::ShaderProgram::getId() const {
    return _programId;
}