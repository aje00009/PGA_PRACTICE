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
        ModelEditor,
        MaterialEditor,
        RenderMode
    };

    /**
     * @enum ModelEditType Enum for every type of transformation that can be applied to a model
     */
    enum class ModelEditType
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
     * @enum LightType Represents the different types of light existing in the application
     */
    enum class LightType
    {
        AMBIENT_LIGHT = 0,
        POINT_LIGHT = 1,
        DIRECTIONAL_LIGHT = 2,
        SPOT_LIGHT = 3
    };

    /**
     * @struct ModelEditPackage Encapsulates the data to be sent when modifying a model
     */
    struct ModelEditPackage
    {
        int modelId;
        ModelEditType type;
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

    struct LightPackage {
        int lightId;
        bool deleteLight;
        LightType type;

        std::string name;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 position;
        glm::vec3 direction;
        float angle;
        float exp;
    };

    //CONSTANTES
    constexpr int NUM_LIGHTS = 4;

}
#endif //PGA_PRACTICA_TYPES_H