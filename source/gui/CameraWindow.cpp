#include "imgui.h"

#include "CameraWindow.h"

#include "../rendering/Renderer.h"

//Definition of the instance for CameraWindow
PAG::CameraWindow* PAG::CameraWindow::instance = nullptr;

PAG::CameraWindow::CameraWindow() : GUIElement("Camera management") {
}

/**
 * @brief Method that warns all listeners for the events of CameraWindow
 */
void PAG::CameraWindow::warnListeners() const {
    for (auto listener : _listeners) {
        listener->wakeUp(WindowType::Camera, _movement);
    }
}

PAG::CameraWindow::~CameraWindow() {
    if (instance) {
        delete instance;
        instance = nullptr;
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
    if (visible) {
        if (ImGui::Begin(title.c_str(), &visible)) {
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
        }

        //Clipping planes
        ImGui::Separator();
        ImGui::Text("Clipping planes");

        Camera *camera = PAG::Renderer::getCamera();

        if (camera) {
            //Obtain current values
            float zNear = camera->getZnear();
            float zFar = camera->getZFar();
            bool changed = false;

            if (ImGui::DragFloat("Near plane", &zNear, 0.1f, 0.1f, zFar - 1.0f, "%.2f")) {
                if (zNear < 0.1f) zNear = 0.1f;
                changed = true;
            }

            if (ImGui::DragFloat("Far Plane", &zFar, 1.0f, zNear + 1.0f, 10000.0f, "%.1f")) {
                changed = true;
            }

            if (changed) {
                camera->setZNear(zNear);
                camera->setZFar(zFar);
            }
        }

        ImGui::End();
    }
}

PAG::CameraMovement PAG::CameraWindow::getSelectedMovement() {
    return instance->_movement;
}
