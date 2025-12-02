#include "RenderModeWindow.h"

#include "imgui.h"

//Definition of the instance
PAG::RenderModeWindow* PAG::RenderModeWindow::instance = nullptr;

/**
 * @brief Method that wakes up all listeners waiting for an event of this GUI window
 */
void PAG::RenderModeWindow::warnListeners() const {
    for (auto listener: _listeners) {
        listener->wakeUp(WindowType::RenderMode, _renderMode);
    }
}

/**
 * @brief Method that creates (first time is called) and returns the only instance of this class
 * @return The only instance for this class
 */
PAG::RenderModeWindow * PAG::RenderModeWindow::getInstance() {
    if (!instance)
        instance = new PAG::RenderModeWindow();
    return instance;
}

/**
 * @brief Method that draws the GUI window related to the possible render mode in the application
 */
void PAG::RenderModeWindow::render() {
    ImGui::Begin("Render mode window");

    if (ImGui::Button("SOLID",ImVec2(100,30))) {
        _renderMode = RenderMode::SOLID;
        warnListeners();
    }

    ImGui::SameLine();

    if (ImGui::Button("WIREFRAME",ImVec2(100,30))) {
        _renderMode = RenderMode::WIREFRAME;
        warnListeners();
    }

    ImGui::SameLine();

    if (ImGui::Button("TEXTURE",ImVec2(100,30))) {
        _renderMode = RenderMode::TEXTURE;
        warnListeners();
    }

    ImGui::End();
}
