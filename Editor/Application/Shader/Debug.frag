#version 430 core
out vec4 FragColor;
uniform vec4 drawColor;

void main()
{
    FragColor = drawColor;
}