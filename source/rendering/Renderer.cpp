#include <cstdarg>
#include <fstream>

#include "glad/glad.h"
#include "imgui.h"

#include "Renderer.h"
#include "../gui/ManagerGUI.h"

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
    delete _activeCamera;
    _activeCamera = nullptr;

    delete[] _bgColor;
    _bgColor = nullptr;

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
            break;
        }

        case WindowType::ModelLoader: {
            std::va_list args;
            va_start(args, t);

            const char* filePath = va_arg(args, const char*);

            va_end(args);

            if (!_activeShaderProgram) {
                Logger::getInstance()->addMessage("Error: No active shader program, please select one");
                break;
            }

            try {
                std::string pathStr(filePath);

                auto newModel = std::make_unique<Model>(_activeShaderProgram,pathStr);

                _models.push_back(std::move(newModel));


                _activeModel = _models.back().get();

                Logger::getInstance()->addMessage("Model " + pathStr + " loaded successfully");
            }catch (const std::runtime_error& e) {
                _activeModel = nullptr;
                throw e;
            }

            break;
        }

    case WindowType::ModelTransformation: {
                std::va_list args;
                va_start(args, t);
                // Extraemos el "paquete" de datos completo
                TransformPackage package = va_arg(args, TransformPackage);
                va_end(args);

                auto& model = _models[package.modelId];

                switch (package.type)
                {
                case TransformType::TRANSLATE:
                    {
                        model->translate(package.transf);
                        break;
                    }
                case TransformType::ROTATE:
                    {
                        model->rotate(package.transf,glm::radians(package.angleDegrees));
                        break;
                    }
                case TransformType::SCALE:
                    {
                        model->scale(package.transf);
                        break;
                    }
                case TransformType::RESET:
                    {
                        model->resetModelMatrix();
                        break;
                    }
                }

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
    glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );

    if (_activeModel) {
        const ShaderProgram* shaderProgram = _activeModel->getShaderProgram();

        shaderProgram->use();

        //Let shader program know the uniforms
        const auto model = _activeModel->getModelMatrix();
        const glm::mat4 view = _activeCamera->getViewMatrix();
        const glm::mat4 projection = _activeCamera->getProjectionMatrix();

        _activeShaderProgram->setUniformMat4("model",model);
        _activeShaderProgram->setUniformMat4("view",view);
        _activeShaderProgram->setUniformMat4("projection",projection);

        _activeModel->draw();
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
    glClearColor ( _bgColor[0], instance->_bgColor[1], instance->_bgColor[2], instance->_bgColor[3] );
    glEnable ( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE );
}

std::vector<std::string> PAG::Renderer::getModelNames() const
{
    std::vector<std::string> modelNames;

    modelNames.reserve(_models.size());

    for (const auto& model: _models)
    {
        modelNames.push_back(model->getModelName());
    }

    return modelNames;
}
