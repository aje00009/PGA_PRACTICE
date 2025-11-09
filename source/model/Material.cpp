#include "Material.h"

PAG::Material::Material(const std::string &name): _name(name) {
}

glm::vec3 PAG::Material::getAmbientColor() const { return _ambientColor; }

void PAG::Material::setAmbientColor(const glm::vec3 &ambient_color) { _ambientColor = ambient_color; }

glm::vec3 PAG::Material::getDiffuseColor() const { return _diffuseColor; }

void PAG::Material::setDiffuseColor(const glm::vec3 &diffuse_color) { _diffuseColor = diffuse_color; }

glm::vec3 PAG::Material::getSpecularColor() const { return _specularColor; }

void PAG::Material::setSpecularColor(const glm::vec3 &specular_color) { _specularColor = specular_color; }

float PAG::Material::getShininess() const { return _shininess; }

void PAG::Material::setShininess(const float shininess) { _shininess = shininess; }

void PAG::Material::setName(const std::string &name) {
    _name = name;
}

std::string PAG::Material::getName() const {
    return _name;
}
