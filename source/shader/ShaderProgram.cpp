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

    delete vs;
    vs = nullptr;

    delete fs;
    fs = nullptr;
}

/**
 * @brief Method that tells OpenGL to use the program id of this object
 */
void PAG::ShaderProgram::use() const {
    glUseProgram(_programId);
}

/**
 * @Method to send a uniform of type mat4 to the shader
 * @param uniformName Name of the uniform to link it to shader
 * @param matrix Matrix of type mat4 to be passed as uniform
 */
void PAG::ShaderProgram::setUniformMat4(const std::string &uniformName, const glm::mat4 &matrix) const {
    GLint position = glGetUniformLocation(_programId, uniformName.c_str());

    if ( position != -1 )
        glUniformMatrix4fv(position, 1, GL_FALSE, glm::value_ptr(matrix));
}

/**
 * @Method to send a uniform of type vec3 to the shader
 * @param uniformName Name of the uniform to link it to shader
 * @param vector Vector of type vec3 to be passed as uniform
 */
void PAG::ShaderProgram::setUniformVec3(const std::string &uniformName, const glm::vec3 &vector) const {
    GLuint position = glGetUniformLocation(_programId, uniformName.c_str());

    if ( position != -1 )
        glUniform3fv(position, 1, glm::value_ptr(vector));
}

/**
 * @Method to send a uniform of type float to the shader
 * @param uniformName Name of the uniform to link it to shader
 * @param value Float to be passed as uniform
 */
void PAG::ShaderProgram::setUniformFloat(const std::string &uniformName, float value) const {
    GLuint position = glGetUniformLocation(_programId, uniformName.c_str());

    if ( position != -1 )
        glUniform1f(position, value);
}

/**
 * @brief Getter that returns the id of the shader program
 * @return The id of the shader program
 */
GLuint PAG::ShaderProgram::getId() const {
    return _programId;
}

/**
 * @brief Method that returns the index of a subroutine given its name (implementation)
 * @param name Name of the subroutine (implementation)
 * @return The index of the implementation of the subroutine
 */
GLuint PAG::ShaderProgram::getSubroutineIndex(const std::string &name) const {
    // Esta función nos da el ID de una implementación [cite: 71]
    GLuint index = glGetSubroutineIndex(_programId, GL_FRAGMENT_SHADER, name.c_str());
    if (index == GL_INVALID_INDEX) {
        throw std::runtime_error("Subroutine with name " + name + " does not exist.");
    }
    return index;
}

/**
 * @brief Method that activates an implementation of a subroutine
 * @param subroutineIndex Index of an implementation of the subroutine
 */
void PAG::ShaderProgram::activateSubroutine(GLuint subroutineIndex) const {
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutineIndex);
}

