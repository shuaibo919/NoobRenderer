#version 460 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoords;


// Forward Rendering to support different ssr effects.

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 viewSpaceFragPos;

void main()
{
    vec4 vsFragPos = (view * model * vec4(aPos, 1.0));
    viewSpaceFragPos = vsFragPos.xyz;
    gl_Position = projection * vsFragPos;
}
