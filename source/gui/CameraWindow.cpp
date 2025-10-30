
#include "CameraWindow.h"

#include "imgui.h"

//Definition of the instance for CameraWindow
PAG::CameraWindow* PAG::CameraWindow::instance = nullptr;

/**
 * @brief Method that warns all listeners for the events of CameraWindow
 */
void PAG::CameraWindow::warnListeners() const {
    for (auto listener : _listeners) {
        listener->wakeUp(WindowType::Camera, _movement);
    }
}

/**
 * @brief Method that creates (first time is called) and returns the only instance for this class
 * @return The only instance for this class
 */
PAG::CameraWindow * PAG::CameraWindow::getInstance() {
    if (!instance) {
        instance = new CameraWindow();
    }
    return instance;
}

/**
 * @brief Method that renders the camera controls window
 */
void PAG::CameraWindow::render() {
    ImGui::Begin("Camera control");

    const char* items[] = { "Pan", "Tilt", "Dolly", "Orbit" };
    ImGui::Combo("Type of movement", &move, items, IM_ARRAYSIZE(items));

    ImGui::Separator();

    if (ImGui::Button("Reset")) {
        _movement = CameraMovement::RESET;
        warnListeners();
    }

    ImGui::Separator();

    switch (move) {
        // Case 0: PAN
        case 0:
            _movement = CameraMovement::PAN;
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
            _movement = CameraMovement::TILT;
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
            _movement = CameraMovement::DOLLY;
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
            _movement = CameraMovement::ORBIT;
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

PAG::CameraMovement PAG::CameraWindow::getSelectedMovement() {
    return instance->_movement;
}
