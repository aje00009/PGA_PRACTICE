#version 410

// Data from model
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoords;

// Data to fragment shader
out vec3 vColor;
out vec3 FragPos_World; // World position for vertex
out vec3 Normal_World;  // Normal vector for vertex
out vec2 TexCoords; // Texture coordinates for vertex

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Final position of the vertex (after projection)
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Pass necessary data

    // Position of vertex on world coordinates
    FragPos_World = vec3(model * view * vec4(aPos, 1.0));

    // Normal transformation to avoid problems
    Normal_World = vec3(transpose(inverse(model*view))) * aNormal;

    // Pasamos el resto de datos
    vColor = aColor;
    TexCoords = aTexCoords;
}


