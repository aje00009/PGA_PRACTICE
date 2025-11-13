#include "LightProperties.h"

PAG::LightProperties::LightProperties(glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is, glm::vec3 pos,
        glm::vec3 direction, glm::vec3 angle, glm::vec3 shininess)
{
    _Ia = Ia;
    _Id = Id;
    _Is = Is;
    _pos = pos;
    _direction = direction;
    _angle = angle;
    _shininess = shininess;
}

[[nodiscard]] glm::vec3 PAG::LightProperties::getIa() const
{
    return _Ia;
}

void PAG::LightProperties::setIa(const glm::vec3& ia)
{
    _Ia = ia;
}

[[nodiscard]] glm::vec3 PAG::LightProperties::getId() const
{
    return _Id;
}

void PAG::LightProperties::setId(const glm::vec3& id)
{
    _Id = id;
}

[[nodiscard]] glm::vec3 PAG::LightProperties::getIs() const
{
    return _Is;
}

void PAG::LightProperties::setIs(const glm::vec3& is)
{
    _Is = is;
}

[[nodiscard]] glm::vec3 PAG::LightProperties::getPos() const
{
    return _pos;
}

void PAG::LightProperties::setPos(const glm::vec3& pos)
{
    _pos = pos;
}

[[nodiscard]] glm::vec3 PAG::LightProperties::getDirection() const
{
    return _direction;
}

void PAG::LightProperties::setDirection(const glm::vec3& direction)
{
    _direction = direction;
}

[[nodiscard]] glm::vec3 PAG::LightProperties::getAngle() const
{
    return _angle;
}

void PAG::LightProperties::setAngle(const glm::vec3& angle)
{
    _angle = angle;
}

[[nodiscard]] glm::vec3 PAG::LightProperties::getShininess() const
{
    return _shininess;
}

void PAG::LightProperties::setShininess(const glm::vec3& shininess)
{
    _shininess = shininess;
}