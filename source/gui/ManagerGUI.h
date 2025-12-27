#ifndef PGA_PRACTICA_GUI_H
#define PGA_PRACTICA_GUI_H

#include <vector>
#include <GLFW/glfw3.h>

#include "GUIElement.h"

/**
 * @class ManagerGUI Class that manages general aspects of the GUI (initialization, windows management, destruction of objects...)
 */
namespace PAG {
    class ManagerGUI {
    private:
        static ManagerGUI* instance;
        std::vector<GUIElement*> _windows;

        ManagerGUI();

        void drawMenuItem (const std::string& label, const std::string& windowTitle) const;

    public:
        ~ManagerGUI();
        static ManagerGUI* getInstance();

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