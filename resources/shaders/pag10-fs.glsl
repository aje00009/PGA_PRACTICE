#version 410

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
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

//Samplers
uniform sampler2D texSampler;
uniform sampler2D normalMap;

//************ Definition of subroutine for diffuse source (TEXTURE/MATERIAL) *****************//

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

//************ Definition of subroutine for normal source (VERTEX/NORMAL MAP) *****************//
subroutine vec3 fNormalSource();
subroutine uniform fNormalSource uNormalSource;

//Geometric normal
subroutine (fNormalSource)
vec3 normalFromVertex(){
    return vec3(0.0, 0.0, 1.0);
}

//Normal mapping
subroutine (fNormalSource)
vec3 normalFromMap() {
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;

    normal = normal * 2.0 - 1.0;

    return normalize(normal);
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
    vec3 N = uNormalSource();

    vec3 ViewPosTangent = fs_in.TBN * vec3(0.0);
    vec3 FragPosTangent = fs_in.TBN * fs_in.FragPos;

    vec3 V = normalize(ViewPosTangent - FragPosTangent);

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
    vec3 lightPosTangent = fs_in.TBN * uLight.position;
    vec3 FragPos_Tangent  = fs_in.TBN * fs_in.FragPos;

    vec3 L = normalize(lightPosTangent - FragPos_Tangent);

    //Calculate attenuation by distance
    float attenuation = calculateAttenuationFactor(uLight.position, fs_in.FragPos);

    return attenuation * calculateDiffSpec(L);
}

subroutine (fLightType)
vec3 directionalLight(){
    vec3  lightDirTangent = normalize(fs_in.TBN * uLight.direction);

    vec3 L = normalize(-lightDirTangent);

    return calculateDiffSpec(L);
}

subroutine (fLightType)
vec3 spotLight(){
    vec3 lightPosTangent = fs_in.TBN * uLight.position;
    vec3 lightDirTangent = normalize(fs_in.TBN * uLight.direction);
    vec3 fragPosTangent  = fs_in.TBN * fs_in.FragPos;

    vec3 L_notNorm = lightPosTangent - fragPosTangent;
    vec3 L = normalize(L_notNorm);
    float distance = length(L_notNorm);

    // Distance attenuation
    float distanceAttenuation = calculateAttenuationFactor(uLight.position, fs_in.FragPos);


    // Angle calculation
    float cosTheta = dot(-L, lightDirTangent);
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