#include "imgui.h"

#include "LoggerWindow.h"
#include "../utils/Logger.h"

//Definition of the instance
PAG::LoggerWindow* PAG::LoggerWindow::instance = nullptr;

PAG::LoggerWindow::LoggerWindow(): GUIElement("Logger") { }

PAG::LoggerWindow::~LoggerWindow() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

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
    if (visible) {
        if (ImGui::Begin(title.c_str(), &visible)) {

            // Clean log console button
            if (ImGui::Button("Clean")) {
                Logger::getInstance()->clear();
            }

            // Checkbox on/off autoscroll
            ImGui::SameLine();
            static bool autoScroll = true;
            ImGui::Checkbox("Auto-scroll", &autoScroll);

            ImGui::Separator();

            ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            ImGui::TextUnformatted(Logger::getInstance()->getMessages().c_str());

            if (autoScroll) {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }
}