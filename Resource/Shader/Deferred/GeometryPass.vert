#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 invTModel;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    vec4 wsPos = model * vec4(aPos, 1.0f);
    FragPos = wsPos.xyz;
    Normal = normalize(mat3(invTModel) * aNormal);
    TexCoords = aTexCoords;
    gl_Position = projection * view * wsPos;
}