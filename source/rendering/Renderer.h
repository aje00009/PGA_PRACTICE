#ifndef PGA_PRACTICA_RENDERER_H
#define PGA_PRACTICA_RENDERER_H

#include <memory>

#include "../camera/Camera.h"
#include "../light/Light.h"
#include "../utils/Listener.h"
#include "../model/Model.h"

/**
 * @class Renderer Class that will render objects in the application, manage different other objects (shaderPrograms, cameras, models...)
 */
namespace PAG {
    class Renderer: public Listener {
        private:
            //General
            static Renderer* instance; ///< Instance of Renderer
            float *_bgColor; ///< Background color of the window
            RenderMode _renderMode = RenderMode::WIREFRAME; ///< Renderer mode of the models
            bool _normalMapping = true; ///< Selectable normal mapping

            //Shadow map
            const unsigned int SHADOWMAP_WIDTH = 1024; ///< Width of the shadow map
            const unsigned int SHADOWMAP_HEIGHT = 1024; ///< Height of the shadow map

            GLuint _depthMapFBO = 0; ///< FBO for shadows
            GLuint _depthMapTex = 0; ///< Texture to include depth

            //Shader programs
            std::vector<std::pair<std::string, std::unique_ptr<ShaderProgram>>> _shaderPrograms; ///< Set of shader programs loaded in the application
            ShaderProgram* _activeShaderProgram = nullptr; ///< Current shader program being executed

            //Models
            std::vector<std::unique_ptr<Model>> _models; ///< Set of models loaded in the application

            //Materials
            std::vector<std::unique_ptr<Material>> _materials; ///< Set of materials created in the application

            //Lights
            std::vector<std::unique_ptr<Light>> _lights; ///< Set of lights created in the application

            //Textures
            std::vector<std::unique_ptr<Texture>> _textures; ///< Set of textures created in the application

            //Cameras
            Camera* _activeCamera = nullptr; ///< Current active camera

            Renderer();
        public:
            virtual ~Renderer();
            static Renderer* getInstance();
            static void initialize(float aspectRatio);

            void wakeUp(WindowType t, ...) override;

            static void error_callback(int error, const char* description);
            static void framebuffer_size_callback(int width, int height);
            static void scroll_callback(double xoffset, double yoffset);
            static void cursor_pos_callback(CameraMovement movement, double deltaX, double deltaY);

            void refresh() const;

            static void getInfoGL();
            void initializeOpenGL() const;

            void createShadowMap();

            [[nodiscard]] std::vector<std::string> getModelNames() const;

            [[nodiscard]] std::vector<std::string> getMaterialNames() const;
            [[nodiscard]] Material* getMaterial(int index) const;
            [[nodiscard]] std::vector<std::string> getLightNames() const;
            [[nodiscard]] Light* getLight(int index) const;

            [[nodiscard]] Texture* getTexture(const std::string& path) const;
            [[nodiscard]] std::string getTextureModel(int modelId) const;

            void setNormalMapping(bool normalMapping);
            [[nodiscard]] bool getNormalMapping() const;

    };
}



#endif //PGA_PRACTICA_RENDERER_H