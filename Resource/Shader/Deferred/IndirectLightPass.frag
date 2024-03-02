#version 460 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D uColorMap;
uniform sampler2D uSsrMap;
uniform sampler2D uSsaoMap;
uniform sampler2D uAmbientColorMap;
uniform sampler2D uVolumetric; // 


void main()
{
    vec3 direct_color = texture(uColorMap,TexCoords).rgb;
    float transparency = texture(uColorMap,TexCoords).a;
    vec3 ambient =  texture(uAmbientColorMap,TexCoords).rgb * texture(uSsaoMap,TexCoords).r;
    vec3 volumetric = texture(uVolumetric,TexCoords).rgb;

    FragColor = vec4( direct_color + volumetric + ambient + texture(uSsrMap,TexCoords).rgb, transparency);
}   

float LinearizeDepth(float depth, float pNear, float pFar)
{
    float z = depth * 2.0 - 1.0; // 回到NDC
    return (2.0 * pNear * pFar) / (pFar + pNear - z * (pFar - pNear));    
}
