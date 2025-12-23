#version 410

// Data from model
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;

uniform mat3 normalMatrix;
uniform mat4 modelView;
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);

    //Position of vertex in view space
    vs_out.FragPos = vec3(modelView * vec4(aPos, 1.0));

    vs_out.TexCoords = aTexCoords;

    // TBN Calculation
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    vec3 B = normalize(cross(N, T));

    vs_out.TBN = transpose(mat3(T,B,N));
}


