#include "Shader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

PAG::Shader::Shader(const std::string& filePath, GLenum type) : _type(type) {
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        throw std::runtime_error("Error opening shader file: " + filePath);
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    compile(shaderStream.str());
}

PAG::Shader::~Shader() {
    if (_id != 0) {
        glDeleteShader(_id);
    }
}

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

GLuint PAG::Shader::getId() const {
    return _id;
}