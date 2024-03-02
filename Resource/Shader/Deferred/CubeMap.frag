#version 460 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;
uniform sampler2D gStencil;
uniform vec2 viewportSize;

void main()
{		
    vec2 screenTexcoord = vec2(gl_FragCoord.x / viewportSize.x,  gl_FragCoord.y / viewportSize.y);
    if(texture(gStencil, screenTexcoord).r == 1) 
        discard;
    vec3 envColor = texture(environmentMap, WorldPos).rgb;
    FragColor = vec4(envColor, 1.0);
}