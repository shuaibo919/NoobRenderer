#version 450 core

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform LightInfo {
    vec3 pos;
} light;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

void main() 
{
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(light.pos - fragPos);      
    float diff = max(dot(norm, lightDir), 0.0);
    outColor =  diff * texture(texSampler, fragTexCoord);
}