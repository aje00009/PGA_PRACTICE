
#include "ShaderLoaderWindow.h"
#include <imgui.h>

//Definition of the only instance of this class
PAG::ShaderLoaderWindow* PAG::ShaderLoaderWindow::instance = nullptr;

/**
 * @brief Method that creates (if it is called for the first time) and returns the only instance for this class
 * @return The only instance for this class
 */
PAG::ShaderLoaderWindow* PAG::ShaderLoaderWindow::getInstance() {
    if (! instance)
        instance = new ShaderLoaderWindow();
    return instance;
}

/**
 * @brief Method that warns all listeners that have subscribed to the uploading shader event
 */
void PAG::ShaderLoaderWindow::warnListeners() const {
    for (Listener* listener : _listeners) {
        listener->wakeUp(WindowType::ShaderLoad,nameShader);
    }
}

/**
 * @brief Method that renders the shader loader window
 */
void PAG::ShaderLoaderWindow::render() {
    ImGui::Begin("Shader Loader Window");

    ImGui::InputText("##",nameShader,ImGuiInputTextFlags_AutoSelectAll);

    if (ImGui::Button("Load Shader")) {
        warnListeners();
    }

    // Finaliza la definición de la ventana.
    ImGui::End();
}
