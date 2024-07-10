#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;
uniform mat4 mvp;

void main()
{
    TexCoords = aTexCoord;
    gl_Position = mvp * vec4(aPos, 1.0);
}