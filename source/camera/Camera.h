//
// Created by alber on 09/10/2025.
//

#ifndef PGA_PRACTICA_CAMERA_H
#define PGA_PRACTICA_CAMERA_H

#include <glm/glm.hpp>

namespace PAG {
    class Camera {
    private:
        glm::vec3 _position;
        glm::vec3 _lookAt;
        glm::vec3 _up;

        float _fov;
        float _aspect;
        float _zNear;
        float _zFar;

    public:
        Camera(float aspectRatio, glm::vec3 position = {0,0,5}, glm::vec3 lookAt = {0,0,0},
            glm::vec3 up = {0,1,0}, float fov = 45, float zNear = 0.1, float zFar = 100);

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

        //Movement
        void orbit(float offX, float offY);
        void pan(float angle);
        void tilt(float angle);
        void dolly(float offX, float offZ);
        void zoom(float off);

        void setAspectRatio(float aspectRatio);
    };
}

#endif //PGA_PRACTICA_CAMERA_H