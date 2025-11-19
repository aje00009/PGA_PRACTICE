#ifndef PGA_PRACTICA_SPOTLIGHTAPPLICATOR_H
#define PGA_PRACTICA_SPOTLIGHTAPPLICATOR_H
#include "LightApplicator.h"

namespace PAG {
    class SpotLightApplicator: public LightApplicator {
        public:
            void applyLight(LightProperties* properties, ShaderProgram* shaderProgram) override {
                GLuint subroutineIndex = shaderProgram->getSubroutineIndex("spotLight");
                shaderProgram->activateSubroutine(subroutineIndex,"uLightType");

                shaderProgram->setUniformVec3("uLight.position",properties->getPos());
                shaderProgram->setUniformVec3("uLight.direction",properties->getDirection());
                shaderProgram->setUniformVec3("uLight.diffuse",properties->getId());
                shaderProgram->setUniformVec3("uLight.specular",properties->getIs());
                shaderProgram->setUniformFloat("uLight.angle",properties->getAngle());
                shaderProgram->setUniformFloat("uLight.exponent",properties->getExponent());

                shaderProgram->setUniformFloat("uLight.c0", properties->getC0());
                shaderProgram->setUniformFloat("uLight.c1", properties->getC1());
                shaderProgram->setUniformFloat("uLight.c2", properties->getC2());
            }
    };
}

#endif //PGA_PRACTICA_SPOTLIGHTAPPLICATOR_H