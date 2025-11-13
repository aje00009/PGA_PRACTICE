//
// Created by albertoje04 on 13/11/25.
//

#ifndef PGA_PRACTICA_LIGHTAPPLICATOR_H
#define PGA_PRACTICA_LIGHTAPPLICATOR_H

#include "LightProperties.h"
#include "../shader/ShaderProgram.h"

namespace PAG
{
    class LightApplicator
    {
    public:
        virtual ~LightApplicator() = default;
        virtual void applyLight(LightProperties properties, ShaderProgram shaderProgram) = 0;
    };
}
#endif //PGA_PRACTICA_LIGHTAPPLICATOR_H