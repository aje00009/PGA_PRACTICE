#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ManagerGUI.h"

//Definition of the instace
PAG::ManagerGUI* PAG::ManagerGUI::instance = nullptr;

/**
 * @brief Default constructor
 */
PAG::ManagerGUI::ManagerGUI() = default;

void PAG::ManagerGUI::drawMenuItem(const std::string &label, const std::string &windowTitle) const {
    for (auto* window : _windows) {
        if (window->getTitle() == windowTitle) {
            if (ImGui::MenuItem(label.c_str(), nullptr, &window->getVisible())) {

            }
            return;
        }
    }
}

PAG::ManagerGUI::~ManagerGUI() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

/**
 * @brief Method that creates an instance of this class (first time is called) and returns it
 * @return The only instance for this class
 */
PAG::ManagerGUI* PAG::ManagerGUI::getInstance() {
    if (!instance)
        instance = new ManagerGUI();
    return instance;
}

/**
 * @brief Method that initialises ImGui parameters
 * @param window Main window for the application
 */
void PAG::ManagerGUI::initialize(GLFWwindow *window) {

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;



    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init();

}

/**
 * @brief Method that initialises a new frame of the GUI before rendering it
 */
void PAG::ManagerGUI::initializeNewFrame() {

    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

}

/**
 * @brief Method that renders a new GUI frame
 */
void PAG::ManagerGUI::renderNewFrame() {

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

/**
 * @brief Method that destroys all ImGui objects used in the application
 */
void PAG::ManagerGUI::destroyImGuiObjects() {

    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

}

/**
 * @brief Method that adds a window to the collection of windows the manager stores
 * @param window Window that we want to add to the GUI
 */
void PAG::ManagerGUI::addWindow(GUIElement* window) {
    _windows.push_back(window);
}

/**
 * @brief Method that renders all windows the manager is storing at the moment
 */
void PAG::ManagerGUI::drawAllWindows() const {
    // 1. Main menu
    if (ImGui::BeginMainMenuBar()) {

        // FILE MENU
        if (ImGui::BeginMenu("File")) {
            drawMenuItem("Import model", "Model loader");
            drawMenuItem("Load shader", "Shader loader");
            ImGui::Separator();

            ImGui::EndMenu();
        }

        //  SCENE MENU
        if (ImGui::BeginMenu("Scene")) {
            drawMenuItem("Model manager", "Model manager");
            drawMenuItem("Light manager", "Light manager");
            drawMenuItem("Camera settings", "Camera management");
            drawMenuItem("Background color", "Background color");
            ImGui::EndMenu();
        }

        //  RENDER MENU
        if (ImGui::BeginMenu("Render")) {
            drawMenuItem("Render mode", "Render mode");
            drawMenuItem("Material editor", "Material editor");
            ImGui::EndMenu();
        }

        //  UTILITY MENU
        if (ImGui::BeginMenu("Utility")) {
            drawMenuItem("Logger", "Logger");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // 2. Render only visible windows
    for (auto* window : _windows) {
        if (window->getVisible()) {
            window->render();
        }
    }
}
