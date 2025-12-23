#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"

#include <vector>

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
 * @brief Method to send a uniform of type mat4 to the shader
 * @param uniformName Name of the uniform to link it to shader
 * @param matrix Matrix of type mat4 to be passed as uniform
 */
void PAG::ShaderProgram::setUniformMat4(const std::string &uniformName, const glm::mat4 &matrix) const {
    GLint position = glGetUniformLocation(_programId, uniformName.c_str());

    if ( position != -1 )
        glUniformMatrix4fv(position, 1, GL_FALSE, glm::value_ptr(matrix));
}

/**
 * @brief Method to send a uniform of type vec3 to the shader
 * @param uniformName Name of the uniform to link it to shader
 * @param vector Vector of type vec3 to be passed as uniform
 */
void PAG::ShaderProgram::setUniformVec3(const std::string &uniformName, const glm::vec3 &vector) const {
    GLuint position = glGetUniformLocation(_programId, uniformName.c_str());

    if ( position != -1 )
        glUniform3fv(position, 1, glm::value_ptr(vector));
}

void PAG::ShaderProgram::setUniformMat3(const std::string &uniformName, const glm::mat3 &matrix) const {
    GLuint position = glGetUniformLocation(_programId, uniformName.c_str());

    if ( position != -1 )
        glUniformMatrix3fv(position, 1, GL_FALSE, glm::value_ptr(matrix));
}

/**
 * @brief Method to send a uniform of type float to the shader
 * @param uniformName Name of the uniform to link it to shader
 * @param value Float to be passed as uniform
 */
void PAG::ShaderProgram::setUniformFloat(const std::string &uniformName, float value) const {
    GLuint position = glGetUniformLocation(_programId, uniformName.c_str());

    if ( position != -1 )
        glUniform1f(position, value);
}

/**
 * @brief Method that sends a uniform of type "int" to the shader
 * @param uniformName Name of the uniform in the shader
 * @param value Value of the uniform
 */
void PAG::ShaderProgram::setUniformInt(const std::string& uniformName, int value) const
{
    GLint position = glGetUniformLocation(_programId, uniformName.c_str());
    if (position != -1)
        glUniform1i(position, value);
}

/**
 * @brief Getter that returns the id of the shader program
 * @return The id of the shader program
 */
GLuint PAG::ShaderProgram::getId() const {
    return _programId;
}

/**
 * @brief Method that ask openGL for all existing subroutine information
 */
void PAG::ShaderProgram::queryStoreSubroutineInfo() {
    _subroutineUniformLocations.clear();
    _subroutineState.clear();
    _subroutineIndices.clear(); // Limpiamos el nuevo mapa

    // Obtain name of uniforms
    glGetProgramStageiv(_programId, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &_numActiveSubroutineUniforms);

    if (_numActiveSubroutineUniforms > 0) {
        GLchar name[256];
        GLsizei length;
        for (int i = 0; i < _numActiveSubroutineUniforms; ++i) {
            glGetActiveSubroutineUniformName(_programId, GL_FRAGMENT_SHADER, i, 256, &length, name);
            std::string uniformName(name);
            GLint location = glGetSubroutineUniformLocation(_programId, GL_FRAGMENT_SHADER, name);

            _subroutineUniformLocations[uniformName] = location;
            _subroutineState[location] = 0;
        }
    }

    // Obtain implementations of subroutines
    GLint numActiveSubroutines = 0;
    glGetProgramStageiv(_programId, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINES, &numActiveSubroutines);

    if (numActiveSubroutines > 0) {
        GLchar subName[256];
        for (int i = 0; i < numActiveSubroutines; i++) {
            // Obtain name of the subroutine
            glGetActiveSubroutineName(_programId, GL_FRAGMENT_SHADER, i, 256, nullptr, subName);

            // Save the pair nameImplementation-Index
            _subroutineIndices[std::string(subName)] = i;
        }
    }
}

/**
 * @brief Method that returns the index of a subroutine given its name (implementation)
 * @param name Name of the subroutine (implementation)
 * @return The index of the implementation of the subroutine
 */
GLuint PAG::ShaderProgram::getSubroutineIndex(const std::string &name) const {
    GLuint index = glGetSubroutineIndex(_programId, GL_FRAGMENT_SHADER, name.c_str());
    if (index == GL_INVALID_INDEX) {
        throw std::runtime_error("Subroutine with name " + name + " does not exist.");
    }
    return index;
}

/**
 * @brief Method that saves the state of a subroutine given its name and the name of the implementation we want to activate
 * @param implementationName Name of the implementation of the subroutine
 * @param uniformName Name of the uniform of the subroutine in the shader
 */
void PAG::ShaderProgram::activateSubroutine(const std::string& implementationName, const std::string& uniformName)
{
    // Search for index of implementation
    auto itImpl = _subroutineIndices.find(implementationName);
    if (itImpl == _subroutineIndices.end()) {
        return;
    }
    GLuint implementationIndex = itImpl->second;

    // Search location of uniform (subroutine uniform)
    auto itLoc = _subroutineUniformLocations.find(uniformName);
    if (itLoc != _subroutineUniformLocations.end()) {
        GLint location = itLoc->second;

        // Assign implementation
        _subroutineState[location] = implementationIndex;
    }
}

/**
 * @brief Method that applies (confirms) all chosen subroutines (implementations)
 */
void PAG::ShaderProgram::applySubroutines() {
    if (_numActiveSubroutineUniforms == 0) return;

    std::vector<GLuint> indices(_numActiveSubroutineUniforms);

    for (auto const& [location, index] : _subroutineState)
    {
        if (location < indices.size()) {
            indices[location] = index;
        }
    }

    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, _numActiveSubroutineUniforms, indices.data());
}

