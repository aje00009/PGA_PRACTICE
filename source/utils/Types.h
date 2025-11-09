#ifndef PGA_PRACTICA_TYPES_H
#define PGA_PRACTICA_TYPES_H
#include <string>
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
        MaterialEditor,
        RenderMode
    };

    /**
     * @enum TransformType Enum for every type of transformation that can be applied to a model
     */
    enum class TransformType
    {
        TRANSLATE,
        ROTATE,
        SCALE,

        MATERIAL_ASSIGN,

        DELETE,
        RESET
    };

    /**
     * @enum RenderMode Represents the different types of modes to render models
     */
    enum class RenderMode {
        SOLID,
        WIREFRAME
    };

    /**
     * @struct ModelEditPackage Encapsulates the data to be sent when modifying a model
     */
    struct ModelEditPackage
    {
        int modelId;
        TransformType type;
        glm::vec3 transf;
        float angleDegrees;
        int materialId;
    };

    /**
     * @struct MaterialEditingPackage Encapsulates the data to be sent when creating/editing a material
     */
    struct MaterialEditingPackage {
        int materialId;
        std::string name;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };

}
#endif //PGA_PRACTICA_TYPES_H