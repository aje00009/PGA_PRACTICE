//
// Created by alber on 17/09/2025.
//
#include "glad/glad.h"
#include "Renderer.h"

#include <cstdarg>
#include <fstream>

#include "ManagerGUI.h"
#include "imgui.h"
#include "Logger.h"

PAG::Renderer* PAG::Renderer::instance = nullptr;

PAG::Renderer::Renderer() = default;

PAG::Renderer::~Renderer() {
    if (idVS != 0)
        glDeleteShader (idVS);
    if (idFS != 0)
        glDeleteShader (idFS);
    if (idSP != 0)
        glDeleteProgram (idSP);
    if (idVBO != 0)
        glDeleteBuffers (1, &idVBO);
    if (idIBO != 0)
        glDeleteBuffers (1, &idIBO);
    if (idVAO != 0)
        glDeleteVertexArrays (1, &idVAO);
}


PAG::Renderer *PAG::Renderer::getInstance() {
    if (!instance) {
        instance = new Renderer();
        instance->_bgColor = new float[4];
        instance->_bgColor[0] = 0.6;
        instance->_bgColor[1] = 0.6;
        instance->_bgColor[2] = 0.6;
        instance->_bgColor[3] = 1.0;
    }
    return instance;
}

void PAG::Renderer::wakeUp(WindowType t, ...) {
    switch (t) {
        case WindowType::BackGround: {
            std::va_list args;
            va_start(args, t);

            _bgColor = (va_arg(args,float*));

            va_end(args);
            break;
        }
    }
}

void PAG::Renderer::error_callback(int error, const char* description) {
    std::string aux(description);
}

void PAG::Renderer::framebuffer_size_callback(int width, int height) {
    glViewport(0, 0, width, height);
}


void PAG::Renderer::scroll_callback(double xoffset, double yoffset) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(xoffset,yoffset);
}

void PAG::Renderer::refresh() const {
    glClearColor(_bgColor[0], _bgColor[1], _bgColor[2], _bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
    glUseProgram(idSP);
    glBindVertexArray(idVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBO);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,NULL);
}

void PAG::Renderer::getInfoGL() const {
    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
    std::stringstream ss;
    ss << glGetString ( GL_RENDERER ) << "\n" << glGetString ( GL_VENDOR ) << "\n" << glGetString ( GL_VERSION )
    << "\n" << glGetString ( GL_SHADING_LANGUAGE_VERSION ) << "\n";

    PAG::Logger::getInstance()->addMessage(ss.str());
}

void PAG::Renderer::initializeOpenGL() const {
    glClearColor ( _bgColor[0], instance->_bgColor[1], instance->_bgColor[3], instance->_bgColor[4] );
    glEnable ( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE );
}

void PAG::Renderer::createShaderProgram(std::string nameShader) {
    std::string pathShaders = "resources/shaders/";

    std::ifstream vertexShaderFile;

    std::string pathVtxShader;
    pathVtxShader.append(pathShaders);
    pathVtxShader.append(nameShader);
    pathVtxShader.append("-vs.glsl");

    vertexShaderFile.open(pathVtxShader);
    if (!vertexShaderFile.is_open()) {
        throw std::runtime_error("Error opening vertex shader file");
    }

    std::stringstream shaderCode;
    shaderCode << vertexShaderFile.rdbuf();
    std::string vertexShaderCode = shaderCode.str();

    vertexShaderFile.close();

    //Creating vertex shader
    idVS = glCreateShader(GL_VERTEX_SHADER);
    if (idVS == 0)
        throw std::runtime_error("Error creating vertex shader");

    const GLchar* vsSourceCode = vertexShaderCode.c_str();
    glShaderSource(idVS, 1, &vsSourceCode, NULL);
    glCompileShader(idVS);

    //Check possible compilation errors on vertex shader
    GLint compResult;
    glGetShaderiv(idVS, GL_COMPILE_STATUS, &compResult);
    if (compResult == GL_FALSE) {
        GLint lengthMssg = 0;
        std::string msg = "";
        glGetShaderiv (idVS, GL_INFO_LOG_LENGTH, &lengthMssg);
        if (lengthMssg > 0) {
            GLchar* msgC = new GLchar[lengthMssg];
            GLint writtenData = 0;
            glGetShaderInfoLog ( idVS, lengthMssg, &writtenData, msgC );
            msg.assign ( msgC );
            delete[] msgC;
            msgC = nullptr;

            throw std::runtime_error(msg);
        }
    }

    std::ifstream fragmShaderFile;

    std::string pathFragShader;
    pathFragShader.append(pathShaders);
    pathFragShader.append(nameShader);
    pathFragShader.append("-fs.glsl");

    fragmShaderFile.open(pathFragShader);
    if (!fragmShaderFile.is_open()) {
        throw std::runtime_error("Error opening fragment shader file");
    }

    std::stringstream fragShaderCodeSS;
    fragShaderCodeSS << fragmShaderFile.rdbuf();
    std::string fragmShaderCode = fragShaderCodeSS.str();

    fragmShaderFile.close();

    idFS = glCreateShader(GL_FRAGMENT_SHADER);
    if (idFS == 0)
        throw std::runtime_error("Error creating fragment shader");

    const GLchar* fsSourceCode = fragmShaderCode.c_str();
    glShaderSource(idFS, 1, &fsSourceCode, NULL);
    glCompileShader(idFS);

    //Check compilation erros on fragment shader
    glGetShaderiv(idFS, GL_COMPILE_STATUS, &compResult);
    if (compResult == GL_FALSE) {
        GLint lengthMssg = 0;
        std::string msg = "";
        glGetShaderiv (idFS, GL_INFO_LOG_LENGTH, &lengthMssg);
        if (lengthMssg > 0) {
            GLchar* msgC = new GLchar[lengthMssg];
            GLint writtenData = 0;
            glGetShaderInfoLog ( idFS, lengthMssg, &writtenData, msgC );
            msg.assign ( msgC );
            delete[] msgC;
            msgC = nullptr;

            throw std::runtime_error(msg);
        }
    }

    idSP = glCreateProgram();
    if ( idSP == 0)
        throw std::runtime_error("Error creating shader program");

    glAttachShader(idSP, idVS);
    glAttachShader(idSP, idFS);
    glLinkProgram(idSP);

    GLint linkStatus = 0;
    glGetProgramiv(idSP, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        GLint lengthMssg = 0;
        std::string msg = "";
        glGetProgramiv(idSP, GL_INFO_LOG_LENGTH, &lengthMssg);
        if (lengthMssg > 0) {
            GLchar* msgC = new GLchar[lengthMssg];
            GLint writtenData = 0;
            glGetProgramInfoLog(idSP,lengthMssg,&writtenData,msgC);
            delete[] msgC;
            msgC = nullptr;

            throw std::runtime_error(msg);
        }
    }
}

void PAG::Renderer::createModel() {
    GLfloat vertices[] = {
        -.5, -.5, 0,
        .5, -.5, 0,
        0, .5, 0,
    };

    GLuint indices[] = {0,1,2};

    glGenVertexArrays(1, &idVAO);
    glBindVertexArray(idVAO);

    glGenBuffers(1, &idVBO);
    glBindBuffer(GL_ARRAY_BUFFER, idVBO);
    glBufferData(GL_ARRAY_BUFFER,9*sizeof(GLfloat),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&idIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*sizeof(GLfloat),indices,GL_STATIC_DRAW);
}