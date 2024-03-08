#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

/* Vertex Output */
out VS_OUT{
    vec2 TexCoords;
    vec3 Normal;
} vs_out;

uniform mat4 model;

void main()
{
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = aNormal;
    gl_Position = model * vec4(aPos, 1.0f);
}