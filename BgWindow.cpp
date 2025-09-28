//
// Created by alber on 28/09/2025.
//

#include "BgWindow.h"
#include <imgui.h>

PAG::BgWindow* PAG::BgWindow::instance = nullptr;

PAG::BgWindow * PAG::BgWindow::getInstance() {
    if (instance == nullptr) {
        instance = new PAG::BgWindow();
    }
    return instance;
}

void PAG::BgWindow::render() {
    ImGui::Begin("Background color");

    if (ImGui::ColorPicker4("Background color", bgColor)) {
        // Solo notificamos si el usuario ha interactuado con el control.
        warnListeners();
    }

    ImGui::End();
}

void PAG::BgWindow::warnListeners() {
    for (Listener* listener : _listeners) {
        listener->wakeUp(WindowType::BackGround, bgColor);
    }
}
