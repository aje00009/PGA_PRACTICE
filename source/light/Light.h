#ifndef PGA_PRACTICA_LIGHT_H
#define PGA_PRACTICA_LIGHT_H
#include <memory>
#include <vector>

#include "../utils/Types.h"
#include "LightApplicator.h"

namespace PAG
{
    class Light
    {
        private:
            LightType _type;

            std::unique_ptr<LightProperties> _properties;
            static std::vector<std::unique_ptr<LightApplicator>> _lightApplicators;
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