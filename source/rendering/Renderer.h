//
// Created by alber on 17/09/2025.
//

#ifndef PGA_PRACTICA_RENDERER_H
#define PGA_PRACTICA_RENDERER_H

#include <iostream>
#include <memory>
#include <vector>

#include "../utils/Listener.h"

/**
 * @class Renderer Class that will render objects in the application
 */
namespace PAG {
    class ShaderProgram;

    class Renderer: public Listener {
        private:
            static Renderer* instance;

            float *_bgColor;

            std::vector<std::pair<std::string, std::unique_ptr<ShaderProgram>>> _shaderPrograms;

            ShaderProgram* _activeShaderProgram = nullptr;

            GLuint idVBOVertex = 0, idVBOColors = 0, idIBOVertex = 0, idVAO = 0;

            Renderer();
        public:
            virtual ~Renderer();
            static Renderer* getInstance();

            void wakeUp(WindowType t, ...) override;

            static void error_callback(int error, const char* description);
            static void framebuffer_size_callback(int width, int height);
            static void scroll_callback(double xoffset, double yoffset);

            void refresh() const;
            static void getInfoGL() ;
            void initializeOpenGL() const;
            void createModel();
    };
}



#endif //PGA_PRACTICA_RENDERER_H