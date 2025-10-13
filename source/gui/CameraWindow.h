//
// Created by alber on 11/10/2025.
//

#ifndef PGA_PRACTICA_CAMERAWINDOW_H
#define PGA_PRACTICA_CAMERAWINDOW_H
#include <vector>

#include "GUIElement.h"

namespace PAG {
    class Listener;

    enum class CameraMovement {
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
        ORBIT_LONGITUDE_RIGHT
    };

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
    };
}

#endif //PGA_PRACTICA_CAMERAWINDOW_H