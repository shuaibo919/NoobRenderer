#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D quadTexture;


void main()
{    
    vec3 col = texture(quadTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}
