#ifndef PGA_PRACTICA_DIRECTIONALLIGHTAPPLICATOR_H
#define PGA_PRACTICA_DIRECTIONALLIGHTAPPLICATOR_H
#include "LightApplicator.h"

/**
 * @class DirectionalLightApplicator Class that implements LightApplicator using directional light
 */
namespace PAG {
    class DirectionalLightApplicator: public LightApplicator {
        public:
            void applyLight(LightProperties* properties, ShaderProgram* shaderProgram) override {
                shaderProgram->activateSubroutine("directionalLight","uLightType");

                shaderProgram->setUniformVec3("uLight.direction",properties->getDirection());
                shaderProgram->setUniformVec3("uLight.diffuse",properties->getId());
                shaderProgram->setUniformVec3("uLight.specular",properties->getIs());
            }
    };
}

#endif //PGA_PRACTICA_DIRECTIONALLIGHTAPPLICATOR_H