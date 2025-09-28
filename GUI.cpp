//
// Created by alber on 22/09/2025.
//

#include "GUI.h"

#include "imgui_impl_opengl3_loader.h"

PAG::GUI* PAG::GUI::instance = nullptr;
float PAG::GUI::s_backgroundColor[4] = { 0.6f, 0.6f, 0.6f, 1.0f };


void PAG::GUI::warnListeners() {
    float *colorData = s_backgroundColor;
    for (Listener* listener: _listeners) {
        listener->wakeUp(WindowType::BackGround, colorData);
    }
}

PAG::GUI::GUI() = default;

void PAG::GUI::addListener(Listener *listener) {
    _listeners.push_back(listener);
}

void PAG::GUI::initialize(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

PAG::GUI* PAG::GUI::getInstance() {
    if (!instance)
        instance = new GUI();
    return instance;
}

void PAG::GUI::initializeNewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void PAG::GUI::renderNewFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void PAG::GUI::drawLoggerWindow(const std::string& messages) {
    if (ImGui::Begin("Logger")) {
        if (ImGui::Button("Limpiar")) {
            Logger::getInstance()->clear();
        }

        ImGui::Separator();

        // Mostramos el contenido actual del log
        ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::TextUnformatted(messages.c_str());

        ImGui::EndChild();
    }

    ImGui::End();
}

void PAG::GUI::drawColorSelectorWindow() {
    ImGui::Begin("BackGround Color");

    ImGui::ColorPicker4("BackGround Color", s_backgroundColor);
    warnListeners();

    ImGui::End();
}

void PAG::GUI::destroyImGuiObjects() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
