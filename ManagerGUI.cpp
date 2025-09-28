//
// Created by alber on 28/09/2025.
//

#include "ManagerGUI.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

PAG::ManagerGUI* PAG::ManagerGUI::instance = nullptr;

// El constructor, getInstance, initialize, etc. se mantienen igual...
PAG::ManagerGUI::ManagerGUI() = default;

PAG::ManagerGUI* PAG::ManagerGUI::getInstance() {
    if (!instance)
        instance = new ManagerGUI();
    return instance;
}

void PAG::ManagerGUI::initialize(GLFWwindow *window) {

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;



    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init();

}

void PAG::ManagerGUI::initializeNewFrame() {

    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

}

void PAG::ManagerGUI::renderNewFrame() {

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void PAG::ManagerGUI::destroyImGuiObjects() {

    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

}

// NUEVOS MÉTODOS
void PAG::ManagerGUI::addWindow(GUIElement* window) {
    _windows.push_back(window);
}

void PAG::ManagerGUI::drawAllWindows() const {
    //Draw all windows we have (background color, controls, logger, etc.)
    for (auto* window : _windows) {
        window->render();
    }
}
