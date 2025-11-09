#version 410

// Data from model
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 vColor;
    vec3 FragPos_World;
    vec3 Normal_World;
    vec2 TexCoords;
} vs_out;

void main()
{
    // Final position of the vertex (after projection)
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vs_out.FragPos_World = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal_World = mat3(transpose(inverse(model))) * aNormal;
    vs_out.vColor = aColor;
    vs_out.TexCoords = aTexCoords;
}


