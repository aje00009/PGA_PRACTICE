#include "Light.h"

#include "AmbientLightApplicator.h"
#include "DirectionalLightApplicator.h"
#include "PointLightApplicator.h"
#include "SpotLightApplicator.h"

std::vector<std::unique_ptr<PAG::LightApplicator>> PAG::Light::_lightApplicators;

PAG::Light::Light(const LightPackage &payloadLight) {
    _type = payloadLight.type;

    _properties = std::make_unique<LightProperties>(payloadLight.name, payloadLight.ambient,payloadLight.diffuse,payloadLight.specular,
        payloadLight.position,payloadLight.direction,payloadLight.angle,payloadLight.exp);
}

void PAG::Light::applyLight(ShaderProgram *sp) {
    auto& applicator = _lightApplicators[static_cast<int>(_type)];

    applicator->applyLight(_properties.get(),sp);
}

PAG::LightProperties * PAG::Light::getLightProperties() const {
    return _properties.get();
}

bool PAG::Light::isEnabled() const {
    return _properties->isEnabled();
}

PAG::LightType PAG::Light::getType() const {
    return _type;
}

void PAG::Light::initializeApplicators() {
    _lightApplicators.resize(NUM_LIGHTS);

    _lightApplicators[static_cast<int>(LightType::AMBIENT_LIGHT)] = std::make_unique<AmbientLightApplicator>();
    _lightApplicators[static_cast<int>(LightType::POINT_LIGHT)] = std::make_unique<PointLightApplicator>();
    _lightApplicators[static_cast<int>(LightType::DIRECTIONAL_LIGHT)] = std::make_unique<DirectionalLightApplicator>();
    _lightApplicators[static_cast<int>(LightType::SPOT_LIGHT)] = std::make_unique<SpotLightApplicator>();
}
