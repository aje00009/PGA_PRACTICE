#version 410

in VS_OUT {
    vec3 FragPos_World;
    vec3 Normal_World;
    vec2 TexCoords;
} fs_in;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float angle;
    float exponent;
    float c0;
    float c1;
    float c2;
};
uniform Light uLight;

//************ Definition of subroutine for diffuse source (TEXTURE/MATERIAL) *****************//
uniform sampler2D texSampler;

// Subroutine for diffuse color
subroutine vec3 fDiffuseSource();
subroutine uniform fDiffuseSource uDiffuseSource;

// Material color
subroutine (fDiffuseSource)
    vec3 colorFromMaterial() {
    return material.diffuse;
}

// Texture color
subroutine (fDiffuseSource)
    vec3 colorFromTexture() {
    return texture(texSampler, fs_in.TexCoords).rgb;
}

//************ Definition of subroutine for light type *****************//
subroutine vec3 fLightType();
subroutine uniform fLightType uLightType;

//Function that calculates the fatt given a light position and the fragment position
float calculateAttenuationFactor(vec3 lightPos, vec3 fragPos) {
float d = length(lightPos - fragPos);
float att = 1.0 / (uLight.c0 + (uLight.c1 * d) + (uLight.c2 * d * d));
return min(att, 1.0);
}

//Function to calculate diffuse + specular value
vec3 calculateDiffSpec (vec3 L){
    vec3 N = normalize(fs_in.Normal_World);
    vec3 V = normalize(-fs_in.FragPos_World);

    vec3 color = uDiffuseSource();
    vec3 diffuse = uLight.diffuse * color * max(dot(N, L), 0.0);

    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), material.shininess);
    vec3 specular = uLight.specular * material.specular * spec;

    return (diffuse + specular);
}

//Implementations
subroutine (fLightType)
vec3 ambientLight(){
    return uLight.ambient * material.ambient;
}

subroutine (fLightType)
vec3 pointLight(){
    vec3 L = normalize(uLight.position-fs_in.FragPos_World);

    //Calculate attenuation by distance
    float attenuation = calculateAttenuationFactor(uLight.position, fs_in.FragPos_World);

    return attenuation * calculateDiffSpec(L);
}

subroutine (fLightType)
vec3 directionalLight(){
    vec3 L = normalize(-uLight.direction);

    return calculateDiffSpec(L);
}

subroutine (fLightType)
vec3 spotLight(){
    vec3 D = normalize(uLight.direction);
    vec3 L_notNorm = uLight.position - fs_in.FragPos_World;
    vec3 L = normalize(L_notNorm);
    float distance = length(L_notNorm);

    // Distance attenuation
    float distanceAttenuation = calculateAttenuationFactor(uLight.position, fs_in.FragPos_World);


    // Angle calculation
    float cosTheta = dot(-L, D);
    float cosOuter = cos(radians(uLight.angle));
    float cosInner = cos(radians(uLight.angle * 0.80));


    // Border softness
    float spotSoftness = smoothstep(cosOuter, cosInner, cosTheta);


    // Light fade on spotlight
    float hotspot = pow(max(0.0, cosTheta), uLight.exponent);


    // Final result (diffuse + specular default) * (distance attenuation fatt) * (border softness) * (exponent light fade)
    return calculateDiffSpec(L) * distanceAttenuation * spotSoftness * hotspot;
}

//************ Definition of subroutine for render mode *****************//
subroutine vec4 fCalculateColor();

// Uniform of subroutine
subroutine uniform fCalculateColor uChosenMethod;

// Implementations of subroutine
subroutine (fCalculateColor)
vec4 solidColor()
{
    vec3 iluminatedColor = uLightType();
    return vec4(iluminatedColor, 1.0);

}

subroutine (fCalculateColor)
vec4 wireframeColor()
{
    return vec4(1.0, 1.0, 1.0, 1.0);
}

// Calling subroutine
void main()
{
    FragColor = uChosenMethod();
}