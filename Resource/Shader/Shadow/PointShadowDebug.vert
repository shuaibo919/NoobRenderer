#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 model;

void main()
{
    TexCoords = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = vec4(FragPos, 1.0);
}