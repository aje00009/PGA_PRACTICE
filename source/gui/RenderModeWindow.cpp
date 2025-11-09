#include "RenderModeWindow.h"

#include "imgui.h"

//Definition of the instance
PAG::RenderModeWindow* PAG::RenderModeWindow::instance = nullptr;

void PAG::RenderModeWindow::warnListeners() const {
    for (auto listener: _listeners) {
        listener->wakeUp(WindowType::RenderMode, _renderMode);
    }
}

PAG::RenderModeWindow * PAG::RenderModeWindow::getInstance() {
    if (!instance)
        instance = new PAG::RenderModeWindow();
    return instance;
}

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

    ImGui::End();
}
