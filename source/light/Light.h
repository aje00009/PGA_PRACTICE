#ifndef PGA_PRACTICA_LIGHT_H
#define PGA_PRACTICA_LIGHT_H
#include <vector>

#include "../utils/Types.h"
#include "LightApplicator.h"

namespace PAG
{
    class Light
    {
        LightType type;
        std::vector<LightApplicator> applicators;
    };
}

#endif //PGA_PRACTICA_LIGHT_H