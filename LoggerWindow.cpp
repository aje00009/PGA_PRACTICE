
#include "LoggerWindow.h"
#include "Logger.h"
#include <imgui.h>

PAG::LoggerWindow* PAG::LoggerWindow::instance = nullptr;

PAG::LoggerWindow * PAG::LoggerWindow::getInstance() {
    if (instance == nullptr)
        instance = new PAG::LoggerWindow();
    return instance;
}

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
