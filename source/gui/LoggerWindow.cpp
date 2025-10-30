
#include "LoggerWindow.h"

#include <imgui.h>
#include "../utils/Logger.h"

//Definition of the instance
PAG::LoggerWindow* PAG::LoggerWindow::instance = nullptr;

/**
 * @brief Gets the only instance of this class and create it if it is the first time the function is called
 * @return The instance of this class itself
 */
PAG::LoggerWindow * PAG::LoggerWindow::getInstance() {
    if (instance == nullptr)
        instance = new PAG::LoggerWindow();
    return instance;
}

/**
 * @brief Method that renders the logger window in the GUI
 */
void PAG::LoggerWindow::render() {
    if (ImGui::Begin("Logger")) {
        if (ImGui::Button("Limpiar")) {
            Logger::getInstance()->clear();
        }

        ImGui::Separator();

        ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        // Obtenemos los mensajes directamente del Logger.
        ImGui::TextUnformatted(Logger::getInstance()->getMessages().c_str());

        ImGui::EndChild();
    }
    ImGui::End();
}
