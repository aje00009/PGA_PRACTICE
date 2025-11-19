#ifndef PGA_PRACTICA_LIGHT_H
#define PGA_PRACTICA_LIGHT_H
#include <memory>
#include <vector>

#include "../utils/Types.h"
#include "LightApplicator.h"

/**
 * @class Light Class that encapsulates all information and functionality about lights
 */
namespace PAG
{
    class Light
    {
        private:
            LightType _type; ///< Type of light

            std::unique_ptr<LightProperties> _properties; ///< Light properties (diffuse, ambient, position, etc.)
            static std::vector<std::unique_ptr<LightApplicator>> _lightApplicators; ///< Shared vector of 4 implementations of LightApplicator
        public:
            Light(const LightPackage& payloadLight);
            virtual ~Light() = default;

            void applyLight(ShaderProgram *sp);

            LightProperties* getLightProperties() const;
            bool isEnabled() const;
            LightType getType() const;


            static void initializeApplicators();
    };
}

#endif //PGA_PRACTICA_LIGHT_H