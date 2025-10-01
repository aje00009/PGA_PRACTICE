//
// Created by alber on 17/09/2025.
//

#ifndef PGA_PRACTICA_RENDERER_H
#define PGA_PRACTICA_RENDERER_H

#include <iostream>
#include <GLFW/glfw3.h>

#include "Listener.h"

namespace PAG {
    class Renderer: public Listener {
        private:
            static Renderer* instance;

            float *_bgColor;

            GLuint idVS = 0; //ID vertex shader
            GLuint idFS = 0; //ID fragment shader
            GLuint idSP = 0; //ID shader program
            GLuint idVAO = 0; //ID vertex array object
            GLuint idVBOVertex = 0; //ID vertex buffer object for vertices
            GLuint idVBOColors = 0; //ID vertex buffer object for color
            GLuint idIBOVertex = 0; //ID index buffer object vertices

            Renderer();
        public:
            virtual ~Renderer();
            static Renderer* getInstance();

            virtual void wakeUp(WindowType t, ...) override;

            static void error_callback(int error, const char* description);
            static void framebuffer_size_callback(int width, int height);
            static void scroll_callback(double xoffset, double yoffset);

            void refresh() const;
            void getInfoGL() const;
            void initializeOpenGL() const;
            void createShaderProgram(std::string nameShader);
            void createModel();
    };
}



#endif //PGA_PRACTICA_RENDERER_H