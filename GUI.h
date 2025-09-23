//
// Created by alber on 22/09/2025.
//

#ifndef PGA_PRACTICA_GUI_H
#define PGA_PRACTICA_GUI_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Logger.h"

namespace PAG {
    class GUI {
        private:
            static GUI* instance;
            static float s_backgroundColor[4]; // RGBA

            GUI();
        public:
            static void initialize(GLFWwindow *window);
            static GUI* getInstance();
            static void initializeNewFrame();
            static void renderNewFrame();
            static void drawLoggerWindow(const std::string& messages);
            static void drawColorSelectorWindow();
            static void destroyImGuiObjects();
    };
}


#endif //PGA_PRACTICA_GUI_H