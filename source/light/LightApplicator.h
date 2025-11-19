#ifndef PGA_PRACTICA_LIGHTAPPLICATOR_H
#define PGA_PRACTICA_LIGHTAPPLICATOR_H

#include "LightProperties.h"
#include "../shader/ShaderProgram.h"

/**
 * @class LightApplicator Interface class to use Strategy design pattern
 */
namespace PAG
{
    class LightApplicator
    {
    public:
        virtual ~LightApplicator() = default;
        virtual void applyLight(LightProperties* properties, ShaderProgram* shaderProgram) = 0; //Method to be implemented by different applicators
    };
}
#endif //PGA_PRACTICA_LIGHTAPPLICATOR_H