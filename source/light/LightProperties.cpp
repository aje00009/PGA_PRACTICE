#include "LightProperties.h"

PAG::LightProperties::LightProperties(std::string name, glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is, glm::vec3 pos,
        glm::vec3 direction, float angle, float s)
{
    _name = name;
    _Ia = Ia;
    _Id = Id;
    _Is = Is;
    _pos = pos;
    _direction = direction;
    _angle = angle;
    _s = s;
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

[[nodiscard]] float PAG::LightProperties::getAngle() const
{
    return _angle;
}

void PAG::LightProperties::setAngle(float angle)
{
    _angle = angle;
}

[[nodiscard]] float PAG::LightProperties::getExponent() const
{
    return _s;
}

void PAG::LightProperties::setExponent(float s)
{
    _s = s;
}

std::string PAG::LightProperties::getName() const {
    return _name;
}

void PAG::LightProperties::setName(const std::string &name) {
    this->_name = name;
}

bool PAG::LightProperties::isEnabled() const {
    return _on;
}

void PAG::LightProperties::setEnable(const bool on) {
    this->_on = on;
}

void PAG::LightProperties::setAttenuation(float c0, float c1, float c2)
{
    _c0 = c0;
    _c1 = c1;
    _c2 = c2;
}

float PAG::LightProperties::getC0() const
{
    return _c0;
}

float PAG::LightProperties::getC1() const
{
    return _c1;
}

float PAG::LightProperties::getC2() const
{
    return _c2;
}
