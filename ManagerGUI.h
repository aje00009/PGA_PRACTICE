//
// Created by alber on 28/09/2025.
//

#ifndef PGA_PRACTICA_GUI_H
#define PGA_PRACTICA_GUI_H


#include <vector>
#include <GLFW/glfw3.h>

#include "GUIelement.h"

namespace PAG {
    class ManagerGUI {
    private:
        static ManagerGUI* instance;
        std::vector<GUIElement*> _windows;

        ManagerGUI();

    public:
        static ManagerGUI* getInstance();

        //Initialization/destruction of GUI
        static void initialize(GLFWwindow* window);
        static void initializeNewFrame();
        static void renderNewFrame();
        static void destroyImGuiObjects();

        //Manipulate windows
        void addWindow(GUIElement* window);
        void drawAllWindows() const;
    };
}


#endif //PGA_PRACTICA_GUI_H