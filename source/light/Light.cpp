#include "Light.h"

#include "AmbientLightApplicator.h"
#include "DirectionalLightApplicator.h"
#include "PointLightApplicator.h"
#include "SpotLightApplicator.h"

//Definition of the instance
std::vector<std::unique_ptr<PAG::LightApplicator>> PAG::Light::_lightApplicators;

/**
 * @brief Default constructor
 * @param payloadLight Payload including light properties to be assigned to the light that is being created
 */
PAG::Light::Light(const LightPackage &payloadLight) {
    _type = payloadLight.type;

    _properties = std::make_unique<LightProperties>(payloadLight.name, payloadLight.ambient,payloadLight.diffuse,payloadLight.specular,
        payloadLight.position,payloadLight.direction,payloadLight.angle,payloadLight.exp);
}

/**
 * @brief Method that applies (assigns uniforms and more info) a light into a shader program
 * @param sp Shader program which we want to send the uniforms to
 */
void PAG::Light::applyLight(ShaderProgram *sp) {
    auto& applicator = _lightApplicators[static_cast<int>(_type)];

    applicator->applyLight(_properties.get(),sp);
}

/**
 * @brief Method that returns the properties of a light
 * @return The light properties of the Light
 */
PAG::LightProperties * PAG::Light::getLightProperties() const {
    return _properties.get();
}

/**
 * @brief Method returns a boolean indicating if a light is on/off
 * @return A boolean indicating if a light is on/off
 */
bool PAG::Light::isEnabled() const {
    return _properties->isEnabled();
}

/**
 * @brief Method that returns the type of light
 * @return The type of light
 */
PAG::LightType PAG::Light::getType() const {
    return _type;
}

/**
 * @brief Method that initialises the different applicators using Strategy pattern
 * @post The vector will be filled with the 4 different implementations corresponding to the 4 different lights (ambient, point, spot and directional)
 */
void PAG::Light::initializeApplicators() {
    _lightApplicators.resize(NUM_LIGHTS);

    _lightApplicators[static_cast<int>(LightType::AMBIENT_LIGHT)] = std::make_unique<AmbientLightApplicator>();
    _lightApplicators[static_cast<int>(LightType::POINT_LIGHT)] = std::make_unique<PointLightApplicator>();
    _lightApplicators[static_cast<int>(LightType::DIRECTIONAL_LIGHT)] = std::make_unique<DirectionalLightApplicator>();
    _lightApplicators[static_cast<int>(LightType::SPOT_LIGHT)] = std::make_unique<SpotLightApplicator>();
}
