//
// Created by alber on 11/10/2025.
//

#include "CameraWindow.h"

#include "imgui.h"

PAG::CameraWindow* PAG::CameraWindow::instance = nullptr;

void PAG::CameraWindow::warnListeners() const {
    for (auto listener : listeners) {
        listener->wakeUp(WindowType::Camera, _movement);
    }
}

PAG::CameraWindow * PAG::CameraWindow::getInstance() {
    if (!instance) {
        instance = new CameraWindow();
    }
    return instance;
}

void PAG::CameraWindow::render() {
    ImGui::Begin("Camera control");

    const char* items[] = { "Pan", "Tilt", "Dolly", "Orbit" };
    ImGui::Combo("Type of movement", &move, items, IM_ARRAYSIZE(items));

    ImGui::Separator();

    switch (move) {
        // Case 0: PAN
        case 0:
            if (ImGui::Button("Left")) {
                _movement = CameraMovement::PAN_LEFT;
                warnListeners();
            }

            ImGui::SameLine();
            if (ImGui::Button("Right")) {
                _movement = CameraMovement::PAN_RIGHT;
                warnListeners();
            }
            break;

        // Case 1: TILT
        case 1:
            if (ImGui::Button("Up")) {
                _movement = CameraMovement::TILT_UP;
                warnListeners();
            }

            ImGui::SameLine();
            if (ImGui::Button("Down")) {
                _movement = CameraMovement::TILT_DOWN;
                warnListeners();
            }
            break;

        // Case 2: DOLLY
        case 2:
            if (ImGui::Button("Forward")) {
                _movement = CameraMovement::DOLLY_FORWARD;
                warnListeners();
            }

            ImGui::SameLine();
            if (ImGui::Button("Backward")) {
                _movement = CameraMovement::DOLLY_BACKWARD;
                warnListeners();
            }

            if (ImGui::Button("Left")) {
                _movement = CameraMovement::DOLLY_LEFT;
                warnListeners();
            }

            ImGui::SameLine();
            if (ImGui::Button("Right")) {
                _movement = CameraMovement::DOLLY_RIGHT;
                warnListeners();
            }
            break;

        // Case 3: ORBIT
        case 3:
            ImGui::Text("Latitude:");
            ImGui::SameLine();
            if (ImGui::Button("Up")) {
                _movement = CameraMovement::ORBIT_LATITUDE_UP;
                warnListeners();
            }

            ImGui::SameLine();
            if (ImGui::Button("Down"))  {
                _movement = CameraMovement::ORBIT_LATITUDE_DOWN;
                warnListeners();
            }

            ImGui::Text("Longitude:");
            ImGui::SameLine();
            if (ImGui::Button("Left")) {
                _movement = CameraMovement::ORBIT_LONGITUDE_LEFT;
                warnListeners();
            }

            ImGui::SameLine();
            if (ImGui::Button("Right")) {
                _movement = CameraMovement::ORBIT_LONGITUDE_RIGHT;
                warnListeners();
            }
            break;
    }

    ImGui::End();
}
