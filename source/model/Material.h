#ifndef PGA_PRACTICA_MATERIAL_H
#define PGA_PRACTICA_MATERIAL_H
#include <string>
#include <glm/glm.hpp>

namespace PAG {
    class Material {
    private:
        std::string _name;
        glm::vec3 _ambientColor{1.0f, 1.0f, 1.0f}; ///< Ambient color for material
        glm::vec3 _diffuseColor{1.0f, 1.0f, 1.0f}; ///< Diffuse color for material
        glm::vec3 _specularColor{1.0f, 1.0f, 1.0f}; ///< Specular color for material
        float _shininess = 32.0f; ///< Specular exponent

    public:
        Material(const std::string& name);
        virtual ~Material() = default;

        glm::vec3 getAmbientColor() const;

        void setAmbientColor(const glm::vec3 &ambient_color);

        glm::vec3 getDiffuseColor() const;

        void setDiffuseColor(const glm::vec3 &diffuse_color);

        glm::vec3 getSpecularColor() const;

        void setSpecularColor(const glm::vec3 &specular_color);

        float getShininess() const;

        void setShininess(float shininess);

        void setName(const std::string& name);

        std::string getName() const;
    };
}

#endif //PGA_PRACTICA_MATERIAL_H