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

//Definition of the only instance of the class
PAG::Renderer* PAG::Renderer::instance = nullptr;

/**
 * @brief Default constructor
 */
PAG::Renderer::Renderer() = default;

/**
 * @brief Method that deletes all info about shaders
 */
PAG::Renderer::~Renderer() {
    if (idVBOVertex != 0) glDeleteBuffers(1, &idVBOVertex);
    if (idVBOColors != 0) glDeleteBuffers(1, &idVBOColors);
    if (idIBOVertex != 0) glDeleteBuffers(1, &idIBOVertex);
    if (idVAO != 0) glDeleteVertexArrays(1, &idVAO);
}

/**
 * @brief Method that returns the only instance of this class (and creates it the first time is called)
 * @return The instance of this class
 */
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

/**
 * @brief Method that wakes up a listener of an event and changes something about the scene
 * @param t Type of window that is waking up the listener
 * @param ... Other possible necessary parameters
 */
void PAG::Renderer::wakeUp(WindowType t, ...) {
    switch (t) {
        case WindowType::BackGround: {
            std::va_list args;
            va_start(args, t);

            _bgColor = (va_arg(args,float*));

            va_end(args);
            break;
        }

        case WindowType::ShaderLoad: {
            std::va_list args;
            va_start(args, t);

            _nameShader = (va_arg(args,char*));

            va_end(args);
            break;
        }
    }
}

/**
 * @brief Method that implements the way to handle an error callback
 * @param error Code of the error that has occurred
 * @param description Description of the error that has occurred
 */
void PAG::Renderer::error_callback(int error, const char* description) {
    std::string aux(description);
}

/**
 * @brief Method that implements the way to handle a frame size change callback
 * @param width Width of the visualization window
 * @param height Height of the visualization window
 */
void PAG::Renderer::framebuffer_size_callback(int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * @brief Method that implements the way to handle a scroll callback
 * @param xoffset X axis movement
 * @param yoffset Y axis movement
 */
void PAG::Renderer::scroll_callback(double xoffset, double yoffset) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(xoffset,yoffset);
}

/**
 * @brief Method that refreshes the visualization window
 */
void PAG::Renderer::refresh() const {
    glClearColor(_bgColor[0], _bgColor[1], _bgColor[2], _bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );

    glUseProgram(idSP);
    glBindVertexArray(idVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBOVertex);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,NULL);
}

/**
 * @brief Method that adds a message to the log indicating info aobut OpenGL
 */
void PAG::Renderer::getInfoGL() {
    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
    std::stringstream ss;
    ss << glGetString ( GL_RENDERER ) << "\n" << glGetString ( GL_VENDOR ) << "\n" << glGetString ( GL_VERSION )
    << "\n" << glGetString ( GL_SHADING_LANGUAGE_VERSION ) << "\n";

    PAG::Logger::getInstance()->addMessage(ss.str());
}

/**
 * @brief Method that initialises OpenGL parameteres
 */
void PAG::Renderer::initializeOpenGL() const {
    glClearColor ( _bgColor[0], instance->_bgColor[1], instance->_bgColor[3], instance->_bgColor[4] );
    glEnable ( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE );
}

/**
 * @brief Method that creates a shader program, a vertex and fragment shader in order to render objects later
 * @param nameShader Name of the file (-vs/-fg .glsl) we want to compile as shaders
 */
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
            msg.assign ( msgC );
            delete[] msgC;
            msgC = nullptr;

            throw std::runtime_error(msg);
        }
    }
}

/**
 * @brief Method that defines a model (VAO) and all it's attributes (VBO,IBO) in order to render it
 */
void PAG::Renderer::createModel() {
    /* VBO no entrelazado
    GLfloat vertices[] = {
        -.5, -.5, 0,
        .5, -.5, 0,
        0, .5, 0,
    };

    GLfloat colors[] = {
        1,1,0,
        0,1,0,
        0,0,1
    };*/

    //VBO entrelazado
    GLfloat vertices_color[] = {
        -.5,-.5,0, 1,1,0,
        .5,-.5,0, 0,1,0,
        0,.5,0, 0,0,1
    };

    GLuint indices[] = {0,1,2};

    //VAO
    glGenVertexArrays(1, &idVAO);
    glBindVertexArray(idVAO);

    /* VBO no entrelazado
    // --- VBO vertices ---
    glGenBuffers(1, &idVBOVertex);
    glBindBuffer(GL_ARRAY_BUFFER, idVBOVertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    // --- VBO de colors ---
    glGenBuffers(1, &idVBOColors);
    glBindBuffer(GL_ARRAY_BUFFER, idVBOColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(1);

    // --- IBO (topological connection between vertices) ---
    glGenBuffers(1,&idIBOVertex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBOVertex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    //VBO entrelazado
    glGenBuffers(1, &idVBOVertex);
    glBindBuffer(GL_ARRAY_BUFFER, idVBOVertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &idVBOColors);
    glBindBuffer(GL_ARRAY_BUFFER, idVBOColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // --- IBO (topological connection between vertices) ---
    glGenBuffers(1,&idIBOVertex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBOVertex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}