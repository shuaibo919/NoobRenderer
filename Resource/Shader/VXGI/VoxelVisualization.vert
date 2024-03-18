#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    vec4 wsPos = model * vec4(aPos, 1.0f);
    FragPos = wsPos.xyz;
    gl_Position = projection * view * wsPos;
}