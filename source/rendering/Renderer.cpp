#include <cstdarg>
#include <filesystem>

#include "Renderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../utils/Logger.h"

//Definition of the only instance of the class
PAG::Renderer* PAG::Renderer::instance = nullptr;

const glm::mat4 biasMatrix(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
);

/**
 * @brief Default constructor
 */
PAG::Renderer::Renderer() = default;

void PAG::Renderer::renderShadowMap(Light *light) const {
    if (_shadowMapShader) {
        LightProperties* props = light->getLightProperties();

        //Bind texture and FBO
        glBindFramebuffer(GL_FRAMEBUFFER, props->getShadowMapFBO());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, props->getShadowMapTex());

        //Clear depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        //Change viewport to shadow map dimension
        glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);


        //Adjusting parameters
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        //Shadow acne
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        //Use depth shader
        _shadowMapShader->use();

        //Calculation of light matrices
        glm::mat4 lightProjection;

        float zNear = 1.0f;
        float zFar = 50.0f;

        if (light->getType() == LightType::DIRECTIONAL_LIGHT) {
            lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, zNear, zFar);
        }

        if (light->getType() == LightType::SPOT_LIGHT) {
            zNear = 0.1f;
            lightProjection = glm::perspective(glm::radians(90.0f), (float)SHADOWMAP_WIDTH/(float)SHADOWMAP_HEIGHT, zNear, zFar);
        }

        glm::mat4 lightViewMatrix = glm::lookAt(props->getPos(), props->getPos() + props->getDirection(), glm::vec3(0.0,1.0,0.0));

        glm::mat4 lightSpaceMatrix = lightProjection * lightViewMatrix;
        props->setLightSpaceMatrix(lightSpaceMatrix);

        for (const auto& model: _models) {
            glm::mat4 MVP = lightSpaceMatrix * model->getModelMatrix();

            _shadowMapShader->setUniformMat4("MVP", MVP);
            model->draw();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glViewport(0,0,_width,_height);
        glDepthFunc(GL_LEQUAL);
    }
}

void PAG::Renderer::updateLightsShadowMap() {
    for (auto& light: _lights) {
        if (light->getLightProperties()->castShadows())
            light->getLightProperties()->setShadowUpdate(true);
    }
}

/**
 * @brief Method that deletes all info about buffers of the model
 */
PAG::Renderer::~Renderer() {
    delete _activeCamera;
    _activeCamera = nullptr;

    delete[] _bgColor;
    _bgColor = nullptr;

    if (instance) {
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

/**
 * @brief Method to initialize the instance of Renderer
 * @param width Width of the viewport
 * @param height Height of the viewport
 * @param aspectRatio Aspect ratio of window
 */
void PAG::Renderer::initialize(int width, int height) {
    //Initializing back ground color
    if (!instance) {
        instance = new Renderer();
        instance->_bgColor = new float[4];
        instance->_bgColor[0] = 0.6;
        instance->_bgColor[1] = 0.6;
        instance->_bgColor[2] = 0.6;
        instance->_bgColor[3] = 1.0;

        instance->_width = width;
        instance->_height = height;

        //Initializing main camera
        instance->_activeCamera = new Camera((float)width/ (float) (height));

        //Initialize default material
        instance->_materials.push_back(std::make_unique<Material>("Default material"));

        //Load shadow mapping shader
        instance->_shaderPrograms.emplace_back("resources/shaders/shadow", std::make_unique<ShaderProgram>("resources/shaders/shadow-vs.glsl",
                                                                        "resources/shaders/shadow-fs.glsl"));

        Logger::getInstance()->addMessage("Shadow shaders loaded...");

        instance->_shadowMapShader = instance->_shaderPrograms.back().second.get();

        //Initialize light applicators using Strategy design pattern
        Light::initializeApplicators();
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
                Logger::getInstance()->addMessage("Loading shader program...");
                if (it != _shaderPrograms.end()) {
                    _activeShaderProgram = it->second.get();
                }else {
                    std::string path = "resources/shaders/";
                    path.append(_nameShader);

                    std::string vertexPath = path;
                    std::string fragmentPath = path;

                    vertexPath.append("-vs.glsl");
                    fragmentPath.append("-fs.glsl");

                    _shaderPrograms.emplace_back(_nameShader,std::make_unique<ShaderProgram>(vertexPath,fragmentPath));

                    _activeShaderProgram = _shaderPrograms.back().second.get();
                }

                _activeShaderProgram->use(); //Activate shader program before reading from it

                _activeShaderProgram->queryStoreSubroutineInfo();

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

        case WindowType::RenderMode: {
            std::va_list args;
            va_start(args, t);
            _renderMode = va_arg(args, RenderMode);
            va_end(args);
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
                std::string modelName = std::filesystem::path(pathStr).filename().string();

                bool modelFound = false;
                for (const auto& model: _models) {
                    if (model->getModelName() == modelName) {
                        modelFound = true;
                        break;
                    }
                }

                if (modelFound) {
                    Logger::getInstance()->addMessage("Model '" + modelName + "' already loaded");
                }else {
                    auto newModel = std::make_unique<Model>(_activeShaderProgram,pathStr);

                    float xOffset = static_cast<float>(_models.size()) * 2.0f;
                    newModel->translate(glm::vec3(xOffset, 0.0f, 0.0f));
                    newModel->setMaterial(_materials.front().get());

                    _models.push_back(std::move(newModel));

                    Logger::getInstance()->addMessage("Model '" + modelName + "' loaded successfully");
                }

                updateLightsShadowMap();

            }catch (const std::runtime_error& e) {
                throw;
            }

            break;
        }

    case WindowType::ModelEditor: {
                std::va_list args;
                va_start(args, t);

                ModelEditPackage* package = va_arg(args, ModelEditPackage*);
                va_end(args);

                auto& model = _models[package->modelId];
                bool geometryChanged = false;

                switch (package->type)
                {
                case ModelEditType::TRANSLATE:
                    {
                        model->translate(package->transf);
                        geometryChanged = true;
                        break;
                    }
                case ModelEditType::ROTATE:
                    {
                        model->rotate(package->transf,glm::radians(package->angleDegrees));
                        geometryChanged = true;
                        break;
                    }
                case ModelEditType::SCALE:
                    {
                        model->scale(package->transf);
                        geometryChanged = true;
                        break;
                    }
                case ModelEditType::RESET:
                    {
                        model->resetModelMatrix();
                        geometryChanged = true;
                        break;
                    }
                case ModelEditType::DELETE:
                    {
                        Logger::getInstance()->addMessage("Deleting model '" + _models[package->modelId]->getModelName() + "'");
                        _models.erase(_models.begin() + package->modelId);
                        geometryChanged = true;

                        break;
                    }
                case ModelEditType::MATERIAL_ASSIGN:
                    {
                        if (package->modelId < _models.size()) {
                            Material* mat = nullptr;
                            if (package->materialId >= 0 && package->materialId < _materials.size()) {
                                mat = _materials[package->materialId].get();
                            }
                            _models[package->modelId]->setMaterial(mat);
                        }
                        break;
                    }
                case ModelEditType::TEXTURE_ASSIGN:
                    {
                        if (!package->texturePath.empty())
                        {
                            //Get texture o create it
                            Texture* tex = getTexture(package->texturePath);

                            if (tex)
                            {
                                model->setTexture(tex);
                                Logger::getInstance()->addMessage("Texture assigned to " + model->getModelName());
                            }
                        }
                        break;
                    }
                case ModelEditType::NORMAL_MAP_ASSIGN:
                    {
                        if (!package->texturePath.empty()) {
                            Texture* tex = getTexture(package->texturePath);

                            if (tex) {
                                model->setNormalMap(tex);
                                Logger::getInstance()->addMessage("Normal map assigned to " + model->getModelName());
                            }
                        }
                        break;
                    }
                }

                if (geometryChanged)
                    updateLightsShadowMap();

            break;
        }

        case WindowType::MaterialEditor: {
            std::va_list args;
            va_start(args, t);
            MaterialEditingPackage* package = va_arg(args, MaterialEditingPackage*);
            va_end(args);

            if (package->materialId == -1) {
                // Create new material
                _materials.push_back(std::make_unique<Material>(package->name));

                Material* newMat = _materials.back().get();

                newMat->setDiffuseColor(package->diffuse);
                newMat->setAmbientColor(package->ambient);
                newMat->setSpecularColor(package->specular);
                newMat->setShininess(package->shininess);

            } else if (package->materialId < _materials.size()) {
                // Edit existing material
                Material* mat = _materials[package->materialId].get();
                mat->setName(package->name);
                mat->setDiffuseColor(package->diffuse);
                mat->setAmbientColor(package->ambient);
                mat->setSpecularColor(package->specular);
                mat->setShininess(package->shininess);
            }
            break;
        }

        case WindowType::ManagerLight: {
            std::va_list args;
            va_start(args, t);

            const LightPackage* payload = va_arg(args, LightPackage*);

            va_end(args);

            if (payload->deleteLight) {
                Logger::getInstance()->addMessage("Light erased: " + _lights[payload->lightId]->getLightProperties()->getName());
                _lights.erase(_lights.begin() + payload->lightId);
            }
            else if (payload->lightId == -1) {
                if (payload->name.empty()) {
                    Logger::getInstance()->addMessage("Cannot create a light without a name");
                } else {
                    Logger::getInstance()->addMessage("Creating new light: " + payload->name);

                    _lights.push_back(std::make_unique<Light>(*payload));
                    _lights.back()->createShadowMap(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
                }
            }
            else {
                // Obtenemos las propiedades de la luz existente
                Light* l = _lights[payload->lightId].get();
                LightProperties* props = l->getLightProperties();

                bool wasCasting = props->castShadows();
                bool nowCasting = payload->castShadows;

                props->setName(payload->name);
                props->setEnable(payload->isEnabled);
                props->setIa(payload->ambient);
                props->setId(payload->diffuse);
                props->setIs(payload->specular);
                props->setPos(payload->position);
                props->setDirection(payload->direction);
                props->setAngle(payload->angle);
                props->setExponent(payload->exp);
                props->setAttenuation(payload->c0, payload->c1, payload->c2);
                props->setCastShadows(payload->castShadows);

                if (!wasCasting && nowCasting) {
                    props->setShadowUpdate(true);
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

    if (instance) {
        instance->_width = width;
        instance->_height = height;
    }
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
    // SHADOW MAPPING
    for (const auto& light: _lights) {
        if ( light->isEnabled() && light->getLightProperties()->castShadows() &&
            light->getLightProperties()->hasUpdate()) {
            renderShadowMap(light.get());
            light->getLightProperties()->setShadowUpdate(false);
        }
    }

    glClearColor(_bgColor[0], _bgColor[1], _bgColor[2], _bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. Configuration of render mode
    if (_renderMode == RenderMode::WIREFRAME) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    const glm::mat4 view = _activeCamera->getViewMatrix();
    const glm::mat4 projection = _activeCamera->getProjectionMatrix();

    bool firstLight = true;

    // Light loops
    for (const auto& light: _lights) {
        if (light->isEnabled())
        {
            // Blending configuration
            if (firstLight) {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                firstLight = false;
            } else {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            }

            // Model loop
            for (const auto& model : _models)
            {
                if (model) {

                    ShaderProgram* shaderProgram = model->getShaderProgram();
                    if (shaderProgram)
                    {
                        shaderProgram->use();

                        // A. Apply light
                        light->applyLight(shaderProgram);


                        // B. Uniforms matrices
                        const auto modelMatrix = model->getModelMatrix();

                        glm::mat4 modelView = view * modelMatrix;
                        glm::mat3 normalMatrix = glm::transpose(glm::inverse(modelView));
                        glm::mat4 MVP = projection * modelView;

                        shaderProgram->setUniformMat4("MVP", MVP);
                        shaderProgram->setUniformMat4("modelView", modelView);
                        shaderProgram->setUniformMat3("normalMatrix",normalMatrix);

                        // C. Shadow mapping

                        // No shadow (default)
                        glm::mat4 shadowMatrix = glm::mat4(1.0f);
                        float shadowMin = 1.0f;

                        if (light->getLightProperties()->castShadows()) {
                            //Calculate shadowMatrix
                            shadowMin = 0.1f;
                            shadowMatrix = biasMatrix * light->getLightProperties()->getLightSpaceMatrix() * modelMatrix;

                            glActiveTexture(GL_TEXTURE2);
                            glBindTexture(GL_TEXTURE_2D, light->getLightProperties()->getShadowMapTex());

                            shaderProgram->setUniformInt("shadowMap", 2);
                        }

                        shaderProgram->setUniformFloat("shadowMin", shadowMin);
                        shaderProgram->setUniformMat4("shadowMatrix", shadowMatrix);

                        // D. Materials
                        Material* mat = model->getMaterial();
                        if (mat) {
                            shaderProgram->setUniformVec3("material.diffuse", mat->getDiffuseColor());
                            shaderProgram->setUniformVec3("material.ambient", mat->getAmbientColor());
                            shaderProgram->setUniformVec3("material.specular", mat->getSpecularColor());
                            shaderProgram->setUniformFloat("material.shininess", mat->getShininess());
                        }

                        // E. Subroutines depending on render mode
                        if (_renderMode == RenderMode::WIREFRAME) {
                            shaderProgram->activateSubroutine("wireframeColor", "uChosenMethod");
                        } else {
                            shaderProgram->activateSubroutine("solidColor", "uChosenMethod");
                        }

                        if (_renderMode == RenderMode::TEXTURE && model->hasTexture()) {
                            // Texture color
                            model->getTexture()->bind();
                            shaderProgram->setUniformInt("texSampler", 0);
                            shaderProgram->activateSubroutine("colorFromTexture", "uDiffuseSource");

                            //Normal mapping
                            glActiveTexture(GL_TEXTURE1);
                            if (model->hasNormalMap() && _normalMapping) {
                                model->getNormalMap()->bind();
                                shaderProgram->setUniformInt("normalMapSampler", 1);
                                shaderProgram->activateSubroutine("normalFromMap", "uNormalSource");
                            } else {
                                shaderProgram->activateSubroutine("normalFromVertex","uNormalSource");
                            }

                        } else {
                            // Material color
                            shaderProgram->activateSubroutine("colorFromMaterial", "uDiffuseSource");
                            shaderProgram->activateSubroutine("normalFromVertex","uNormalSource");
                        }

                        // E. Send indices to shader
                        shaderProgram->applySubroutines();

                        // F. Draw model
                        model->draw();
                    }
                }
            }
        }
    }
}

/**
 * @brief Method that adds a message to the log indicating info about OpenGL
 */
void PAG::Renderer::getInfoGL() {
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
    glEnable( GL_MULTISAMPLE );
    glEnable( GL_BLEND );
    glEnable ( GL_DEPTH_TEST );
    glDepthFunc ( GL_LEQUAL );
}

/**
 * @brief Method that returns all model names (model.obj) loaded in the application at the moment
 * @return All model names (string) loaded at the moment in the application
 */
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

/**
 * @return The collection of names of the materials registered in the application
 */
std::vector<std::string> PAG::Renderer::getMaterialNames() const {
    std::vector<std::string> materialNames;

    for (const auto& material: _materials) {
        materialNames.push_back(material->getName());
    }

    return materialNames;
}

/**
 * @brief Method that returns a material given an index (position in array)
 * @param index Position of the material in the array
 * @return The material in position "index"
 */
PAG::Material* PAG::Renderer::getMaterial(int index) const {
    return _materials[index].get();
}

/**
 * @brief Method that returns the name of all lights existing in the application
 * @return The names of all lights existing at the moment
 */
std::vector<std::string> PAG::Renderer::getLightNames() const {
    std::vector<std::string> lightNames;
    for (const auto& light : _lights) {
        lightNames.push_back(light->getLightProperties()->getName());
    }

    return lightNames;
}

/**
 * @brief Method that returns a light of the vector of lights given its index
 * @param index Id of the light in the array
 * @return A pointer to the light in the array
 */
PAG::Light * PAG::Renderer::getLight(int index) const {
    return _lights[index].get();
}

/**
 * @brief Method that creates (first time texture is loaded) and returns a pointer to a texture given its path
 * @param path Path of the texture
 * @return A pointer to the texture in the array
 */
PAG::Texture* PAG::Renderer::getTexture(const std::string& path) const
{
    for (const auto& text: _textures)
    {
        if (text->getPath() == path) return text.get();
    }

    Logger::getInstance()->addMessage("Loading new texture from disc: " + path);

    auto newTex = std::make_unique<Texture>(path);

    instance->_textures.push_back(std::move(newTex));

    return instance->_textures.back().get();
}

/**
 * @brief Method that returns the texture path associated to a specific model
 * @param modelId Id of the model
 * @return The texture associated to the model specified by modelId (or "" if it doesn't have a texture)
 */
std::string PAG::Renderer::getTextureModel(int modelId) const {
    if (!_models.empty() && modelId >= 0 && modelId < _models.size() - 1)
    {
        Model* model = _models[modelId].get();
        if (model->hasTexture()) {
            return model->getTexture()->getPath();
        }
    }

    return "";
}

void PAG::Renderer::setNormalMapping(bool normalMapping) {
    _normalMapping = normalMapping;
}

bool PAG::Renderer::getNormalMapping() const {
    return _normalMapping;
}
