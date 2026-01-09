#ifndef PGA_PRACTICA_CAMERA_H
#define PGA_PRACTICA_CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>

/**
 * @class Camera This class will encapsulate the parameters of a virtual camera and all of its possible movements in the scene
 */
namespace PAG {
    class Camera {
    private:
        glm::vec3 _position; ///< Position of the camera in the scene
        glm::vec3 _lookAt; ///< Point where the camera is looking at
        glm::vec3 _up; ///< Up vector (Y axis)

        float _fov; ///< Width of the angle of the camera
        float _aspect; ///< Width/height ratio
        float _zNear; ///< Near plane of the volume that the camera covers
        float _zFar; ///< Far plane of the volume that the camera covers

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
        void reset();

        float getZnear() const;
        float getZFar() const;
        void setZNear(float zNear);
        void setZFar(float zFar);
    };
}

#endif //PGA_PRACTICA_CAMERA_H