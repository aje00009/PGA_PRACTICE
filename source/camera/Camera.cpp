#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

/**
 * @brief Parameterized constructor of the class Camera
 * @param aspectRatio Width/height ratio
 * @param position Position where the camera will be placed in the scene
 * @param lookAt Point where the camera is going to look at
 * @param up Up vector of the camera (Y axis)
 * @param fov Field of view defined by the angle that represents it
 * @param zNear Near plane of the volume that the camera will cover
 * @param zFar Far plane of the volume that the camera will cover
 */
PAG::Camera::Camera(float aspectRatio, glm::vec3 position, glm::vec3 lookAt, glm::vec3 up, float fov, float zNear,
                    float zFar) {

    _position = position;
    _lookAt = lookAt;
    _up = up;
    _zNear = zNear;
    _zFar = zFar;
    _aspect = aspectRatio;
    _fov = fov;
}

/**
 * @brief Method that returns the view transformation matrix of the camera
 * @return The view transformation matrix for the camera
 */
glm::mat4 PAG::Camera::getViewMatrix() const {
    return glm::lookAt(_position,_lookAt,_up);
}

/**
 * @brief Method that returns the projection transformation matrix of the camera
 * @return The projection transformation matrix for the camera
 */
glm::mat4 PAG::Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(_fov), _aspect, _zNear, _zFar);
}

/**
 * @brief Method that allows orbiting the camera towards the point where it looks (lookAt)
 * @param offX Horizontal displacement (longitude)
 * @param offY Vertical displacement (latitude)
 * @post The position where the camera looks will be changed according to the shift in latitude and longitude
 */
void PAG::Camera::orbit(float offX, float offY) {
    // Define horizontal and vertical movement for orbit movement
    float yaw = -offX;
    float pitch = -offY;

    // Rotation matrices for orbit movement (latitude and longitude)
    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), _up);
    glm::vec3 right = glm::normalize(glm::cross(_lookAt - _position, _up));
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), right);

    // Potential direction calculation
    glm::vec3 currentDirection = _position - _lookAt;
    glm::vec3 potentialDirection = glm::vec3(yawRotation * pitchRotation * glm::vec4(currentDirection, 0.0f));

    glm::vec3 dir_normalized = glm::normalize(potentialDirection);

    // Calculate the cosine of the angle
    float cosAngle = glm::dot(dir_normalized, _up);

    const float limit = 0.999;

    // Checking angle limit
    if (std::abs(cosAngle) < limit) {
        _position = _lookAt + potentialDirection;
    }
}

/**
 * @brief Method that allows rotating the camera itself (using Y axis)
 * @param angle Displacement representing the rotation angle
 * @post The camera will remain at its position, while the point where it looks will be changed horizontally
 */
void PAG::Camera::pan(float angle) {
    float mov = -angle;

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(mov), _up);

    _lookAt = glm::vec3(rotation * glm::vec4(_lookAt - _position, 1.0f)) + _position;
}

/**
 * @brief Method that allows rotating the camera itself (using X axis)
 * @param angle Displacement representing the rotation angle
 * @post The camera will remain at its position, while the point where it looks will be changed vertically
 */
void PAG::Camera::tilt(float angle) {
    float pitch = -angle;

    glm::vec3 right = glm::normalize(glm::cross(_lookAt - _position, _up));

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), right);

    glm::vec3 potentialLookAt = glm::vec3(rotation * glm::vec4(_lookAt - _position, 1.0f)) + _position;

    //Prohibiting looking totally up or down
    glm::vec3 direction = potentialLookAt - _position;

    glm::vec3 normalizedDirection = glm::normalize(direction);

    float cosAngle = glm::dot(normalizedDirection, _up);

    const float limit = 0.999;

    //If both direction and up vector are parallel, the displacement will not be performed
    if (std::abs(cosAngle) < limit) {
        _lookAt = potentialLookAt;
    }
}

/**
 * @brief Method that allow the displacement of the camera along the x and z axis
 * @param offX Displacement on x axis
 * @param offZ Displacement on z axis
 * @post The position of the camera will be changed according to the x and z coordinate displacement (not the point where the camera looks)
 */
void PAG::Camera::dolly(float offX, float offZ) {
    glm::vec3 viewDir = glm::normalize(_lookAt - _position);
    glm::vec3 right = glm::normalize(glm::cross(viewDir, _up));

    glm::vec3 translation = (viewDir * offZ) + (right * offX);

    _position += translation;
    _lookAt += translation;
}

/**
 * @brief Method that allows zooming in or out
 * @param off The quantity of zoom we want to apply
 * @post The angle of the camera will be changed, then the objects of the scene will appear to be closer/farther
 */
void PAG::Camera::zoom(float off) {
    _fov -= off;
    if (_fov < 1.0) _fov = 1.0;
    if (_fov > 90.0) _fov = 90.0;
}

/**
 * @brief Setter of the attribute aspect ratio
 * @param aspectRatio Aspect ratio we want to apply
 */
void PAG::Camera::setAspectRatio(float aspectRatio) {
    this->_aspect = aspectRatio;
}

/**
 * @brief Method that resets the coordinate system of the camera to the default one
 */
void PAG::Camera::reset() {
    _position = {0,0,5};
    _lookAt = {0,0,0};
    _up = {0,1,0};
}

float PAG::Camera::getZnear() const {
    return _zNear;
}

float PAG::Camera::getZFar() const {
    return _zFar;
}
