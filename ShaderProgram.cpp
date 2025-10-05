#include "ShaderProgram.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include <stdexcept>

PAG::ShaderProgram::ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    _programId = glCreateProgram();
    if (_programId == 0) {
        throw std::runtime_error("Error creating shader program.");
    }

    // Los objetos VertexShader y FragmentShader se crean y se destruyen en este ámbito,
    // limpiando sus recursos automáticamente gracias a RAII.
    VertexShader vs(vertexShaderPath);
    FragmentShader fs(fragmentShaderPath);

    glAttachShader(_programId, vs.getId());
    glAttachShader(_programId, fs.getId());
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

    // Los shaders ya no son necesarios una vez enlazados
    glDetachShader(_programId, vs.getId());
    glDetachShader(_programId, fs.getId());
}

PAG::ShaderProgram::~ShaderProgram() {
    if (_programId != 0) {
        glDeleteProgram(_programId);
    }
}

void PAG::ShaderProgram::use() const {
    glUseProgram(_programId);
}

GLuint PAG::ShaderProgram::getId() const {
    return _programId;
}