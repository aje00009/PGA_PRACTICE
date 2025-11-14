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
};
uniform Light uLight;

//************ Definition of subroutine for light type *****************//
subroutine vec3 fLightType();
subroutine uniform fLightType uLightType;

//Function to calculate diffuse + specular value
vec3 calculateDiffSpec (vec3 L){
    vec3 N = normalize(fs_in.Normal_World);
    vec3 V = normalize(-fs_in.FragPos_World);

    vec3 diffuse = uLight.diffuse * material.diffuse * max(dot(N, L), 0.0);

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

    return calculateDiffSpec(L);
}

subroutine (fLightType)
vec3 directionalLight(){
    vec3 L = normalize(-uLight.direction);

    return calculateDiffSpec(L);
}

subroutine (fLightType)
vec3 spotLight(){
    vec3 L = normalize(uLight.position-fs_in.FragPos_World);
    vec3 D = uLight.direction;
    float cosGamma = cos(uLight.angle);
    float cosTheta = dot(-L,D);
    float spotFactor = 1.0;

    if(cosTheta < cosGamma) { spotFactor = 0.0; }

    float attenuation = pow(cosTheta,uLight.exponent);

    return spotFactor * calculateDiffSpec(L);
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