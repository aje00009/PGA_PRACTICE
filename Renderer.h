//
// Created by alber on 17/09/2025.
//

#ifndef PGA_PRACTICA_RENDERER_H
#define PGA_PRACTICA_RENDERER_H

#include <iostream>
#include <GLFW/glfw3.h>

namespace PAG {
    class Renderer {
        private:
            static Renderer* instance;

            GLuint idVS = 0; //ID vertex shader
            GLuint idFS = 0; //ID fragment shader
            GLuint idSP = 0; //ID shader program
            GLuint idVAO = 0; //ID vertex array object
            GLuint idVBO = 0; //ID vertex buffer object
            GLuint idIBO = 0; //ID index buffer object

            Renderer();
        public:
            virtual ~Renderer();
            static PAG::Renderer* getInstance();

            static void error_callback(int error, const char* description);
            static void window_refresh_callback(GLFWwindow* window);
            static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
            static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
            static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

            void refresh() const;
            static void initializeOpenGL();
            void createShaderProgram();
            void createModel();
    };
}



#endif //PGA_PRACTICA_RENDERER_H