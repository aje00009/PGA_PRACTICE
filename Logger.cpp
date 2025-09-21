//
// Created by alber on 17/09/2025.
//

#include "Logger.h"

Logger* Logger::instance = nullptr;

template<typename T>
Logger& Logger::operator<<(const T& smth) {
    messages << smth;
    return *this;
}

Logger* Logger::getInstance() {
    if (!instance)
        instance = new Logger();
    return instance;
}


void Logger::clear() {
    messages.str("");
    messages.clear();
}


void Logger::addMessage(const std::string& message) {
    messages << message << "\n";
}

void Logger::draw() {
    if (ImGui::Begin("log")) {
        if (ImGui::Button("Limpiar")) {
            clear();
        }

        ImGui::Separator();

        // Mostramos el contenido actual del log
        std::string content = messages.str();
        ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::TextUnformatted(content.c_str());

        ImGui::EndChild();
    }

    ImGui::End();
}