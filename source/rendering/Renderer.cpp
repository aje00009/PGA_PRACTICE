//
// Created by alber on 17/09/2025.
//
#include "glad/glad.h"
#include "Renderer.h"

#include <cstdarg>
#include <fstream>

#include "../gui/ManagerGUI.h"
#include "imgui.h"
#include "../gui/CameraWindow.h"
#include "../utils/Logger.h"
#include "../shader/ShaderProgram.h"

//Definition of the only instance of the class
PAG::Renderer* PAG::Renderer::instance = nullptr;

/**
 * @brief Default constructor
 */
PAG::Renderer::Renderer() = default;

/**
 * @brief Method that deletes all info about buffers of the model
 */
PAG::Renderer::~Renderer() {
    if (idVBOVertex != 0) glDeleteBuffers(1, &idVBOVertex);
    if (idVBOColors != 0) glDeleteBuffers(1, &idVBOColors);
    if (idIBOVertex != 0) glDeleteBuffers(1, &idIBOVertex);
    if (idVAO != 0) glDeleteVertexArrays(1, &idVAO);
    if (!instance) {
        delete instance;
        instance = nullptr;
    }
}

/**
 * @brief Method that returns the only instance of this class
 * @return The instance of this class
 */
PAG::Renderer *PAG::Renderer::getInstance() {
    if (!instance)
        throw std::runtime_error("PAG::Renderer::getInstance() called with null instance");
    return instance;
}

void PAG::Renderer::initialize(float aspectRatio) {
    //Initializing back ground color
    if (!instance) {
        instance = new Renderer();
        instance->_bgColor = new float[4];
        instance->_bgColor[0] = 0.6;
        instance->_bgColor[1] = 0.6;
        instance->_bgColor[2] = 0.6;
        instance->_bgColor[3] = 1.0;

        //Initializing main camera
        instance->_activeCamera = new Camera(aspectRatio);
    }
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

            std::string _nameShader = (va_arg(args,char*));

            va_end(args);

            auto it = _shaderPrograms.begin();
            while (it != _shaderPrograms.end()) {
                if (it->first == _nameShader) {
                    break;
                }
               ++it;
            }
            try {
                if (it != _shaderPrograms.end()) {
                    Logger::getInstance()->addMessage("Shader program found, loading...");
                    _activeShaderProgram = it->second.get();
                }else {
                    Logger::getInstance()->addMessage("Shader program not found, creating and loading...");

                    std::string path = "resources/shaders/";
                    path.append(_nameShader);

                    std::string vertexPath = path;
                    std::string fragmentPath = path;

                    vertexPath.append("-vs.glsl");
                    fragmentPath.append("-fs.glsl");

                    _shaderPrograms.emplace_back(_nameShader,std::make_unique<ShaderProgram>(vertexPath,fragmentPath));

                    _activeShaderProgram = _shaderPrograms.back().second.get();
                }
            }catch (const std::runtime_error& e) {
                Logger::getInstance()->addMessage(e.what());
                _activeShaderProgram = nullptr;
            }
             break;
        }

        case WindowType::Camera: {
            std::va_list args;
            va_start(args, t);

            CameraMovement _mov = (va_arg(args,CameraMovement));

            va_end(args);

            if (!_activeCamera) break;

            const float rotationStep = 5.0;
            const float movementStep = 0.2;

            switch (_mov) {
                case CameraMovement::PAN_LEFT:
                    _activeCamera->pan(-rotationStep);
                    break;
                case CameraMovement::PAN_RIGHT:
                    _activeCamera->pan(rotationStep);
                    break;

                case CameraMovement::TILT_UP:
                    _activeCamera->tilt(-rotationStep);
                    break;
                case CameraMovement::TILT_DOWN:
                    _activeCamera->tilt(rotationStep);
                    break;

                case CameraMovement::DOLLY_FORWARD:
                    _activeCamera->dolly(0.0f, movementStep);
                    break;
                case CameraMovement::DOLLY_BACKWARD:
                    _activeCamera->dolly(0.0f, -movementStep);
                    break;
                case CameraMovement::DOLLY_LEFT:
                    _activeCamera->dolly(-movementStep, 0.0f);
                    break;
                case CameraMovement::DOLLY_RIGHT:
                    _activeCamera->dolly(movementStep, 0.0f);
                    break;

                case CameraMovement::ORBIT_LATITUDE_UP:
                    _activeCamera->orbit(0.0f, rotationStep);
                    break;
                case CameraMovement::ORBIT_LATITUDE_DOWN:
                    _activeCamera->orbit(0.0f, -rotationStep);
                    break;
                case CameraMovement::ORBIT_LONGITUDE_LEFT:
                    _activeCamera->orbit(rotationStep, 0.0f);
                    break;
                case CameraMovement::ORBIT_LONGITUDE_RIGHT:
                    _activeCamera->orbit(-rotationStep, 0.0f);
                    break;
                case CameraMovement::RESET:
                    _activeCamera->reset();
                    break;
            }
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
    instance->_activeCamera->zoom(yoffset);
}

void PAG::Renderer::cursor_pos_callback(CameraMovement movement, double deltaX, double deltaY) {
    if (!instance->_activeCamera) {
        throw std::runtime_error("Instance of camera in Renderer is null");
    }

    switch (movement) {
        case CameraMovement::DOLLY: {
            instance->_activeCamera->dolly(deltaX * 0.01,-(deltaY * 0.01));
            break;
        }
        case CameraMovement::PAN: {
            instance->_activeCamera->pan(deltaX * 0.2);
            break;
        }
        case CameraMovement::TILT: {
            instance->_activeCamera->tilt(deltaY * 0.2);
            break;
        }
        case CameraMovement::ORBIT: {
            instance->_activeCamera->orbit(deltaX * 0.2,deltaY * 0.2);
            break;
        }
    }
}

/**
 * @brief Method that refreshes the visualization window
 */
void PAG::Renderer::refresh() const {
    glClearColor(_bgColor[0], _bgColor[1], _bgColor[2], _bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );

    if (_activeShaderProgram) {
        //Activation on shader program
        _activeShaderProgram->use();

        //Let shader program know the uniforms
        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 view = _activeCamera->getViewMatrix();
        glm::mat4 projection = _activeCamera->getProjectionMatrix();

        glm::mat4 mvp = projection * view * model;

        _activeShaderProgram->setUniformMat4("MVP",mvp);

        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBOVertex);
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,NULL);
    }
}

/**
 * @brief Method that adds a message to the log indicating info about OpenGL
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
 * @brief Method that defines a model (VAO) and all it's attributes (VBO,IBO) in order to render it
 */
void PAG::Renderer::createModel() {
    //VBO definition
    // Format: X, Y, Z, R, G, B
    GLfloat vertices_color[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

        // Back face (Verde)
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,

        // Upper face
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

        // Inferior face
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

        // Right face
         0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,

        // Left face
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f
    };

    //IBO Definition
    GLuint indices[] = {
        0, 1, 2,   2, 3, 0,
        4, 5, 6,   6, 7, 4,
        8, 9, 10,  10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    glGenVertexArrays(1, &idVAO);
    glBindVertexArray(idVAO);

    glGenBuffers(1, &idVBOVertex);
    glBindBuffer(GL_ARRAY_BUFFER, idVBOVertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &idIBOVertex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBOVertex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}