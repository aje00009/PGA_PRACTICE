#ifndef PGA_PRACTICA_AMBIENTLIGHTAPPLICATOR_H
#define PGA_PRACTICA_AMBIENTLIGHTAPPLICATOR_H
#include "LightApplicator.h"

/**
 * @class AmbientLightApplicator Class that implements LightApplicator using ambient light
 */
namespace PAG {
    class AmbientLightApplicator: public LightApplicator {
    public:
        void applyLight(LightProperties* properties, ShaderProgram* shaderProgram) override {
            shaderProgram->activateSubroutine("ambientLight","uLightType");

            shaderProgram->setUniformVec3("uLight.ambient",properties->getIa());
        }
    };
}

#endif //PGA_PRACTICA_AMBIENTLIGHTAPPLICATOR_H