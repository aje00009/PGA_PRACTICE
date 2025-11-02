#ifndef PGA_PRACTICA_TYPES_H
#define PGA_PRACTICA_TYPES_H
#include <glm/vec3.hpp>

/**
 * @file Types.h File that contains definition of different general types, enums, data structures, etc.
 */
namespace PAG
{
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
     * @enum WindowType Class for every type of window GUI draws
     */
    enum class WindowType {
        BackGround,
        Camera,
        ShaderLoad,
        ModelLoader,
        ModelTransformation,
    };

    /**
     * @enum TransformType Enum for every type of transformation that can be applied to a model
     */
    enum class TransformType
    {
        TRANSLATE,
        ROTATE,
        SCALE,
        RESET
    };

    struct TransformPackage
    {
        int modelId;
        TransformType type;
        glm::vec3 transf;
        float angleDegrees;
    };
}
#endif //PGA_PRACTICA_TYPES_H