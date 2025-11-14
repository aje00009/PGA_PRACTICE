#ifndef PGA_PRACTICA_AMBIENTLIGHTAPPLICATOR_H
#define PGA_PRACTICA_AMBIENTLIGHTAPPLICATOR_H
#include "LightApplicator.h"

namespace PAG {
    class AmbientLightApplicator: public LightApplicator {
    public:
        void applyLight(LightProperties* properties, ShaderProgram* shaderProgram) override {
            GLuint subroutineIndex = shaderProgram->getSubroutineIndex("ambientLight");
            shaderProgram->activateSubroutine(subroutineIndex,"uLightType");

            shaderProgram->setUniformVec3("uLight.ambient",properties->getIa());
        }
    };
}

#endif //PGA_PRACTICA_AMBIENTLIGHTAPPLICATOR_H