//
// Created by alber on 11/10/2025.
//

#ifndef PGA_PRACTICA_CAMERAWINDOW_H
#define PGA_PRACTICA_CAMERAWINDOW_H

#include "GUIElement.h"

namespace PAG {
    /**
     * @enum CameraMovement Enum that represents the different movements a camera can perform
     */
    enum class CameraMovement {
        //When clicking with left mouse button
        PAN,
        TILT,
        DOLLY,
        ORBIT,

        //Specific for GUI buttons
        PAN_LEFT,
        PAN_RIGHT,

        TILT_UP,
        TILT_DOWN,

        DOLLY_FORWARD,
        DOLLY_BACKWARD,
        DOLLY_RIGHT,
        DOLLY_LEFT,

        ORBIT_LATITUDE_UP,
        ORBIT_LATITUDE_DOWN,
        ORBIT_LONGITUDE_LEFT,
        ORBIT_LONGITUDE_RIGHT,

        RESET
    };

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