#include "LightProperties.h"

/**
 * @brief Parameterized constructor
 * @param name Name of the light
 * @param Ia Ambient intensity
 * @param Id Diffuse intensity
 * @param Is Specular intensity
 * @param pos Position of the light
 * @param direction Direction of the light
 * @param angle Angle of the light
 * @param s Exponent for soft borders
 */
PAG::LightProperties::LightProperties(std::string name, glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is, glm::vec3 pos,
                                      glm::vec3 direction, float angle, float s, bool castShadows): _lightSpaceMatrix(1.0f)
{
    _name = name;
    _Ia = Ia;
    _Id = Id;
    _Is = Is;
    _pos = pos;
    _direction = direction;
    _angle = angle;
    _s = s;

    _castShadows = castShadows;
    _shadowUpdate = true;
    _shadowMapFBO = 0;
    _shadowMapTex = 0;
    _c0 = 1.0f; _c1 = 0.0; _c2 = 0.0;
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
    _shadowUpdate = true;
}

[[nodiscard]] glm::vec3 PAG::LightProperties::getDirection() const
{
    return _direction;
}

void PAG::LightProperties::setDirection(const glm::vec3& direction)
{
    _direction = direction;
    _shadowUpdate = true;
}

[[nodiscard]] float PAG::LightProperties::getAngle() const
{
    return _angle;
}

void PAG::LightProperties::setAngle(float angle)
{
    _angle = angle;
    _shadowUpdate = true;
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

bool PAG::LightProperties::castShadows() const {
    return _castShadows;
}

void PAG::LightProperties::setCastShadows(bool cast_shadows) {
    _castShadows = cast_shadows;
    _shadowUpdate = true;
}

bool PAG::LightProperties::hasUpdate() const {
    return _shadowUpdate;
}

void PAG::LightProperties::setShadowUpdate(bool shadow_update) {
    _shadowUpdate = shadow_update;
}

glm::mat4 PAG::LightProperties::getLightSpaceMatrix() const {
    return _lightSpaceMatrix;
}

void PAG::LightProperties::setLightSpaceMatrix(const glm::mat4 &light_space_matrix) {
    _lightSpaceMatrix = light_space_matrix;
}

unsigned int PAG::LightProperties::getShadowMapFBO() const {
    return _shadowMapFBO;
}

void PAG::LightProperties::setShadowMapFBO(unsigned int shadow_map_fbo) {
    _shadowMapFBO = shadow_map_fbo;
}

unsigned int PAG::LightProperties::getShadowMapTex() const {
    return _shadowMapTex;
}

void PAG::LightProperties::setShadowMapTex(unsigned int shadow_map_tex) {
    _shadowMapTex = shadow_map_tex;
}
