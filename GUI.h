//
// Created by alber on 22/09/2025.
//

#ifndef PGA_PRACTICA_GUI_H
#define PGA_PRACTICA_GUI_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>

#include "Listener.h"
#include "Logger.h"

namespace PAG {
    class GUI {
        private:
            static GUI* instance;
            static float s_backgroundColor[4]; // RGBA

            std::vector<Listener*> _listeners;
            void warnListeners();

            GUI();
        public:
            void addListener(Listener* listener);

            static void initialize(GLFWwindow *window);
            static GUI* getInstance();
            static void initializeNewFrame();
            static void renderNewFrame();
            static void drawLoggerWindow(const std::string& messages);
            void drawColorSelectorWindow();
            static void destroyImGuiObjects();
    };
}


#endif //PGA_PRACTICA_GUI_H