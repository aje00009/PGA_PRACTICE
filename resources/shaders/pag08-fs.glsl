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

float calculateAttenuationFactor(vec3 lightPos, vec3 fragPos) {
    float d = length(lightPos - fragPos);
    float att = 1.0 / (uLight.c0 + (uLight.c1 * d) + (uLight.c2 * d * d));
    return min(att, 1.0);
}

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

    // Calculamos la atenuación
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
// 1. Vectores y Distancia
vec3 D = normalize(uLight.direction);
vec3 L_notNorm = uLight.position - fs_in.FragPos_World;
vec3 L = normalize(L_notNorm);
float distance = length(L_notNorm);

// --- PARTE 1: ATENUACIÓN POR DISTANCIA (Tu fórmula) ---
// Esto hace que la luz pierda fuerza cuanto más lejos esté el objeto
float distanceAttenuation = calculateAttenuationFactor(uLight.position, fs_in.FragPos_World);


// --- PARTE 2: CÁLCULO DEL ÁNGULO ---
float cosTheta = dot(-L, D);
float cosOuter = cos(radians(uLight.angle));
float cosInner = cos(radians(uLight.angle * 0.80)); // Borde interior al 80%


// --- PARTE 3: SUAVIZADO DEL BORDE (Smoothstep) ---
// Esto elimina el corte a negro duro.
float spotSoftness = smoothstep(cosOuter, cosInner, cosTheta);


// --- PARTE 4: EXPONENTE / HOTSPOT (Esto es lo que faltaba) ---
// Esto concentra la luz en el centro del foco.
// Usamos max(0.0, cosTheta) para evitar problemas matemáticos si el ángulo es negativo.
float hotspot = pow(max(0.0, cosTheta), uLight.exponent);


// --- RESULTADO FINAL ---
// Multiplicamos: (Luz Phong) * (Distancia) * (Borde Suave) * (Exponente)
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