
#include "Shader.h"

#include <fstream>
#include <sstream>

/**
 * @brief Parameterized constructor of the class Shader
 * @param filePath Relative path of the shader (vertex or fragment)
 * @param type Type of shader (vertex/fragment)
 * @post The constructor will create vertex/fragment shaders and compile them
 */
PAG::Shader::Shader(const std::string& filePath, GLenum type) : _type(type) {
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        throw std::runtime_error("Error opening shader file: " + filePath);
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    compile(shaderStream.str());
}

/**
 * @brief Destructor of the class Shader
 */
PAG::Shader::~Shader() {
    if (_id != 0) {
        glDeleteShader(_id);
    }
}

/**
 * @brief Method that compiles a shader
 * @param source Source code of the shader (.glsl)
 */
void PAG::Shader::compile(const std::string& source) {
    _id = glCreateShader(_type); //Creating it depending on which type (vertex/fragment) the shader is

    if (_id == 0) {
        throw std::runtime_error("Error creating shader of type " + std::to_string(_type));
    }

    const char* sourceCStr = source.c_str();
    glShaderSource(_id, 1, &sourceCStr, nullptr);
    glCompileShader(_id);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            std::string infoLog(logLength, ' ');
            glGetShaderInfoLog(_id, logLength, nullptr, &infoLog[0]);

            throw std::runtime_error(infoLog);
        }
    }
}

/**
 * @brief Method that returns the id of a shader
 * @return The id of a shader
 */
GLuint PAG::Shader::getId() const {
    return _id;
}