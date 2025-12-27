#include "BgWindow.h"
#include <imgui.h>

//Definition of the only instance of this class
PAG::BgWindow* PAG::BgWindow::instance = nullptr;

PAG::BgWindow::BgWindow(): GUIElement("Background color") { }

PAG::BgWindow::~BgWindow() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

/**
 * @brief Method that creates (first time is called) and returns the only instance for this class
 * @return The instance for this class
 */
PAG::BgWindow * PAG::BgWindow::getInstance() {
    if (instance == nullptr) {
        instance = new BgWindow();
    }
    return instance;
}

/**
 * @brief Overridden (GUIElement) method that renders the window in order to control background colorr
 */
void PAG::BgWindow::render() {
    if (visible) {
        if (ImGui::Begin(title.c_str(), &visible)) {
            if (ImGui::ColorPicker4("Background color", bgColor)) {
                warnListeners();
            }
        }

        ImGui::End();
    }
}

/**
 * @brief Method that warns all suscribers to the event in which the background color is changed
 * @post Renderer will be aware of the color change and will refresh the window to switch to the current selected color
 */
void PAG::BgWindow::warnListeners() {
    for (Listener* listener : _listeners) {
        listener->wakeUp(WindowType::BackGround, bgColor);
    }
}
