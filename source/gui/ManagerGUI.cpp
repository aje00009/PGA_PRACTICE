#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ManagerGUI.h"

//Definition of the instace
PAG::ManagerGUI* PAG::ManagerGUI::instance = nullptr;

/**
 * @brief Default constructor
 */
PAG::ManagerGUI::ManagerGUI() = default;

/**
 * @brief Method to set a custom style to ImGui controls
 */
void PAG::ManagerGUI::setCustomStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Geometry
    style.WindowRounding    = 6.0f;
    style.ChildRounding     = 6.0f;
    style.FrameRounding     = 4.0f;
    style.PopupRounding     = 4.0f;
    style.GrabRounding      = 4.0f;
    style.TabRounding       = 4.0f;
    style.ScrollbarRounding = 8.0f;

    style.FramePadding      = ImVec2(8, 4);
    style.ItemSpacing       = ImVec2(8, 6);
    style.GrabMinSize       = 12.0f;

    // Color
    ImVec4* colors = style.Colors;

    // Background
    colors[ImGuiCol_WindowBg]       = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_PopupBg]        = ImVec4(0.10f, 0.10f, 0.10f, 0.98f);
    colors[ImGuiCol_ChildBg]        = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Headers
    colors[ImGuiCol_Header]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.92f, 0.45f, 0.05f, 1.00f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(0.80f, 0.35f, 0.00f, 1.00f);

    // Buttons (Orange by default)
    colors[ImGuiCol_Button]         = ImVec4(0.92f, 0.45f, 0.05f, 1.00f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(1.00f, 0.55f, 0.10f, 1.00f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.70f, 0.30f, 0.00f, 1.00f);

    // Frames
    colors[ImGuiCol_FrameBg]        = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

    // Tabs
    colors[ImGuiCol_Tab]            = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TabHovered]     = ImVec4(0.92f, 0.45f, 0.05f, 1.00f);
    colors[ImGuiCol_TabActive]      = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TabUnfocused]   = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

    // Window title
    colors[ImGuiCol_TitleBg]        = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgActive]  = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

    // Control elements
    colors[ImGuiCol_CheckMark]      = ImVec4(0.92f, 0.45f, 0.05f, 1.00f);
    colors[ImGuiCol_SliderGrab]     = ImVec4(0.92f, 0.45f, 0.05f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]= ImVec4(0.80f, 0.35f, 0.00f, 1.00f);
    colors[ImGuiCol_ResizeGrip]     = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.45f, 0.05f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]  = ImVec4(0.80f, 0.35f, 0.00f, 1.00f);

    // Extra
    colors[ImGuiCol_Text]           = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_Border]         = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
    colors[ImGuiCol_Separator]      = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.45f, 0.05f, 1.00f);
    colors[ImGuiCol_SeparatorActive]  = ImVec4(0.80f, 0.35f, 0.00f, 1.00f);
}

/**
 * @brief Method that draws a specific item on the menu of the GUI
 * @param label Label of the menu item
 * @param windowTitle Title of the GUI window
 */
void PAG::ManagerGUI::drawMenuItem(const std::string &label, const std::string &windowTitle) const {
    for (auto* window : _windows) {
        if (window->getTitle() == windowTitle) {
            ImGui::MenuItem(label.c_str(), nullptr, &window->getVisible());
            return;
        }
    }
}

/**
 * @brief Destructor of the class
 */
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

    //Set style to custom
    setCustomStyle();

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
