#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
uniform samplerCube depthMap;
uniform vec3 lightPos;
uniform float far_plane;


void main()
{             
    vec3 fragToLight = FragPos - lightPos;
    float closestDepth = texture(depthMap, fragToLight).r;
    FragColor = vec4(vec3(closestDepth / far_plane), 1.0);
    // FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}