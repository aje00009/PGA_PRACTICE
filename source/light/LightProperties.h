#ifndef PGA_PRACTICA_LIGHTPROPERTIES_H
#define PGA_PRACTICA_LIGHTPROPERTIES_H

#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

/**
 * @class LightProperties Class that represents the properties of a light
 */
namespace PAG
{
    class LightProperties
    {
    private:
        std::string _name; ///< Name of the light
        bool _on = true; ///< Light on/off
        bool _castShadows = false; ///< Cast shadow flag
        bool _shadowUpdate = true; ///< Flag to update shadow map

        unsigned int _shadowMapFBO = 0; ///< ID of FBO for depth
        unsigned int _shadowMapTex = 0; ///< ID of depth texture

        glm::mat4 _lightSpaceMatrix; ///< View * projection matrix for this light

        glm::vec3 _Ia; ///< Ambient intensity
        glm::vec3 _Id; ///< Diffuse intensity
        glm::vec3 _Is; ///< Specular intensity
        glm::vec3 _pos; ///< Position of the light (position)
        glm::vec3 _direction; ///< Direction of the light (directional)
        float _angle; ///< Angle of the light (spotlight)
        float _s; ///< Exponent for soft borders
        float _c0; ///< First component for attenuation factor
        float _c1; ///< Second component for attenuation factor
        float _c2; ///< Third component for attenuation factor

    public:
        LightProperties(std::string name, glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is, glm::vec3 pos,
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

        void setAttenuation (float c0, float c1, float c2);

        float getC0() const;

        float getC1() const;

        float getC2() const;

        [[nodiscard]] bool castShadows() const;

        void setCastShadows(bool cast_shadows);

        [[nodiscard]] bool hasUpdate() const;

        void setShadowUpdate(bool shadow_update);

        [[nodiscard]] glm::mat4 getLightSpaceMatrix() const;

        void setLightSpaceMatrix(const glm::mat4 &light_space_matrix);

        [[nodiscard]] unsigned int getShadowMapFBO() const;

        void setShadowMapFBO(unsigned int shadow_map_fbo);

        [[nodiscard]] unsigned int getShadowMapTex() const;

        void setShadowMapTex(unsigned int shadow_map_tex);
    };
}

#endif //PGA_PRACTICA_LIGHTPROPERTIES_H