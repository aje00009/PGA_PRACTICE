#ifndef PGA_PRACTICA_CAMERAWINDOW_H
#define PGA_PRACTICA_CAMERAWINDOW_H

#include "GUIElement.h"

namespace PAG {
    /**
     * @class CameraWindow Class that represents the camera control window in the UI
     */
    class CameraWindow: public GUIElement {
    private:
        static CameraWindow* instance;

        int move = 0;
        CameraMovement _movement;

        CameraWindow() = default;
        void warnListeners() const;

    public:
        static CameraWindow* getInstance();

        void render() override;

        static CameraMovement getSelectedMovement();
    };
}

#endif //PGA_PRACTICA_CAMERAWINDOW_H