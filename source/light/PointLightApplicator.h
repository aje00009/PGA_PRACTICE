#ifndef PGA_PRACTICA_POINTLIGHTAPPLICATOR_H
#define PGA_PRACTICA_POINTLIGHTAPPLICATOR_H
#include "LightApplicator.h"

namespace PAG
{
    class PointLightApplicator: public LightApplicator
    {
        public:
            void applyLight(LightProperties* properties, ShaderProgram* shaderProgram) override
            {
                GLuint subroutineIndex = shaderProgram->getSubroutineIndex("pointLight");
                shaderProgram->activateSubroutine(subroutineIndex,"uLightType");

                shaderProgram->setUniformVec3("uLight.position",properties->getPos());
                shaderProgram->setUniformVec3("uLight.diffuse",properties->getId());
                shaderProgram->setUniformVec3("uLight.specular",properties->getIs());
                shaderProgram->setUniformFloat("uLight.c0", properties->getC0());
                shaderProgram->setUniformFloat("uLight.c1", properties->getC1());
                shaderProgram->setUniformFloat("uLight.c2", properties->getC2());
            }
    };
}

#endif //PGA_PRACTICA_POINTLIGHTAPPLICATOR_H
