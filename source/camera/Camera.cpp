//
// Created by alber on 09/10/2025.
//

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

PAG::Camera::Camera(float aspectRatio, glm::vec3 position, glm::vec3 lookAt, glm::vec3 up, float fov, float zNear,
    float zFar) {

    _position = position;
    _lookAt = lookAt;
    _up = up;
    _zNear = zNear;
    _zFar = zFar;
    _aspect = aspectRatio;
    _fov = glm::radians(fov);
}

glm::mat4 PAG::Camera::getViewMatrix() const {
    return glm::lookAt(_position,_lookAt,_up);
}

glm::mat4 PAG::Camera::getProjectionMatrix() const {
    return glm::perspective(_fov, _aspect, _zNear, _zFar);
}

void PAG::Camera::orbit(float offX, float offY) {
    float yaw = -offX;
    float pitch = -offY;

    //Only allow semicircle movement
    glm::vec3 dir = glm::normalize(_position - _lookAt);
    float currentPitch = glm::degrees(asin(dir.y));
    float maxPitch = 89.0f;
    if (currentPitch + pitch > maxPitch) {
        pitch = maxPitch - currentPitch;
    } else if (currentPitch + pitch < -maxPitch) {
        pitch = -maxPitch - currentPitch;
    }

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), _up);

    glm::vec3 right = glm::normalize(glm::cross(_lookAt - _position, _up));
    rotation = glm::rotate(rotation, glm::radians(pitch), right);

    _position = glm::vec3(rotation * glm::vec4(_position - _lookAt, 1.0f)) + _lookAt;
}

void PAG::Camera::pan(float angle) {
    float mov = -angle;

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(mov), _up);

    // Rotamos el punto de mira alrededor de la posición de la cámara
    _lookAt = glm::vec3(rotation * glm::vec4(_lookAt - _position, 1.0f)) + _position;
}

void PAG::Camera::tilt(float angle) {
    float pitch = -angle;

    glm::vec3 right = glm::normalize(glm::cross(_lookAt - _position, _up));

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), right);

    _lookAt = glm::vec3(rotation * glm::vec4(_lookAt - _position, 1.0f)) + _position;
}

void PAG::Camera::dolly(float offX, float offZ) {
    glm::vec3 viewDir = glm::normalize(_lookAt - _position);
    glm::vec3 right = glm::normalize(glm::cross(viewDir, _up));

    glm::vec3 translation = (viewDir * offZ) + (right * offX);

    _position += translation;
    _lookAt += translation;
}

void PAG::Camera::zoom(float off) {
    _fov -= off;
    _fov = std::max(1.0f, std::min(_fov, 90.0f));
}

