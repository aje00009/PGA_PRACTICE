//
// Created by alber on 02/10/2025.
//

#include "ShaderLoaderWindow.h"

#include "imgui.h"

PAG::ShaderLoaderWindow* PAG::ShaderLoaderWindow::instance = nullptr;

PAG::ShaderLoaderWindow * PAG::ShaderLoaderWindow::getInstance() {
    if (! instance)
        instance = new ShaderLoaderWindow();
    return instance;
}

void PAG::ShaderLoaderWindow::warnListeners() const {
    for (Listener* listener : _listeners) {
        listener->wakeUp(WindowType::ShaderLoad,nameShader);
    }
}

void PAG::ShaderLoaderWindow::render() {
    ImGui::Begin("Shader Loader Window");

    ImGui::InputText("##",nameShader,ImGuiInputTextFlags_AutoSelectAll);

    if (ImGui::Button("Load Shader")) {
        warnListeners();
    }

    // Finaliza la definición de la ventana.
    ImGui::End();
}
