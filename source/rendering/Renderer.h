//
// Created by alber on 17/09/2025.
//

#ifndef PGA_PRACTICA_RENDERER_H
#define PGA_PRACTICA_RENDERER_H

#include <vector>
#include <string>
#include <memory>

#include "../utils/Listener.h"
#include "../shader/ShaderProgram.h"
#include "../model/Model.h"
#include "../camera/Camera.h"
#include "../gui/CameraWindow.h"

/**
 * @class Renderer Class that will render objects in the application, manage different other objects (shaderPrograms, cameras, models...)
 */
namespace PAG {
    class Renderer: public Listener {
        private:
            static Renderer* instance;

            float *_bgColor;

            //Shader programs
            std::vector<std::pair<std::string, std::unique_ptr<ShaderProgram>>> _shaderPrograms;
            ShaderProgram* _activeShaderProgram = nullptr;

            //Models
            std::vector<std::unique_ptr<Model>> _models;
            Model* _activeModel = nullptr;

            //Cameras
            Camera* _activeCamera = nullptr;

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
            static void getInfoGL() ;
            void initializeOpenGL() const;

            void createModel();
    };
}



#endif //PGA_PRACTICA_RENDERER_H