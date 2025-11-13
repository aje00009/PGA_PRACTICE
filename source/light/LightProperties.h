#ifndef PGA_PRACTICA_LIGHTPROPERTIES_H
#define PGA_PRACTICA_LIGHTPROPERTIES_H

#include <string>
#include <glm/vec3.hpp>

namespace PAG
{
    class LightProperties
    {
    private:
        std::string _name;
        bool _on = true;

        glm::vec3 _Ia;
        glm::vec3 _Id;
        glm::vec3 _Is;
        glm::vec3 _pos;
        glm::vec3 _direction;
        float _angle;
        float _s;

    public:
        LightProperties(glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is, glm::vec3 pos,
            glm::vec3 direction, float angle, float s);

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

        [[nodiscard]] float getAngle() const;

        void setAngle(float angle);

        [[nodiscard]] float getExponent() const;

        void setExponent(float s);

        [[nodiscard]] std::string getName() const;

        void setName(const std::string &name);

        [[nodiscard]] bool isEnabled() const;

        void setEnable(const bool on);
    };
}

#endif //PGA_PRACTICA_LIGHTPROPERTIES_H