#ifndef PGA_PRACTICA_DIRECTIONALLIGHTAPPLICATOR_H
#define PGA_PRACTICA_DIRECTIONALLIGHTAPPLICATOR_H
#include "LightApplicator.h"

namespace PAG {
    class DirectionalLightApplicator: public LightApplicator {
        public:
            void applyLight(LightProperties* properties, ShaderProgram* shaderProgram) override {
                GLuint subroutineIndex = shaderProgram->getSubroutineIndex("directionalLight");
                shaderProgram->activateSubroutine(subroutineIndex,"uLightType");

                shaderProgram->setUniformVec3("uLight.direction",properties->getDirection());
                shaderProgram->setUniformVec3("uLight.diffuse",properties->getId());
                shaderProgram->setUniformVec3("uLight.specular",properties->getIs());
            }
    };
}

#endif //PGA_PRACTICA_DIRECTIONALLIGHTAPPLICATOR_H