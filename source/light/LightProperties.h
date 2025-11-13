//
// Created by albertoje04 on 13/11/25.
//

#ifndef PGA_PRACTICA_LIGHTPROPERTIES_H
#define PGA_PRACTICA_LIGHTPROPERTIES_H

#include <glm/vec3.hpp>

namespace PAG
{
    class LightProperties
    {
    private:
        glm::vec3 _Ia;
        glm::vec3 _Id;
        glm::vec3 _Is;
        glm::vec3 _pos;
        glm::vec3 _direction;
        glm::vec3 _angle;
        glm::vec3 _shininess;

    public:
        LightProperties(glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is, glm::vec3 pos,
            glm::vec3 direction, glm::vec3 angle, glm::vec3 shininess);

        [[nodiscard]] glm::vec3 getIa() const;

        void setIa(const glm::vec3& ia);

        [[nodiscard]] glm::vec3 getId() const;

        void setId(const glm::vec3& id);

        [[nodiscard]] glm::vec3 getIs() const;

        void setIs(const glm::vec3& is);

        [[nodiscard]] glm::vec3 getPos() const;

        void setPos(const glm::vec3& pos);

        [[nodiscard]] glm::vec3 getDirection() const;

        void setDirection(const glm::vec3& direction);

        [[nodiscard]] glm::vec3 getAngle() const;

        void setAngle(const glm::vec3& angle);

        [[nodiscard]] glm::vec3 getShininess() const;

        void setShininess(const glm::vec3& shininess);
    };
}

#endif //PGA_PRACTICA_LIGHTPROPERTIES_H