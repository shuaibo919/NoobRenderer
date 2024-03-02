#version 460 core
in vec2 TexCoords;

out vec4 fragColor;

uniform sampler2D uScreenMap;
uniform vec2 uResolution;
uniform vec2 uDirection;

// https://github.com/Experience-Monks/glsl-fast-gaussian-blur
vec3 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
    vec3 color = vec3(0.0);
    vec2 off1 = vec2(1.411764705882353) * direction;
    vec2 off2 = vec2(3.2941176470588234) * direction;
    vec2 off3 = vec2(5.176470588235294) * direction;
    color += texture(image, uv).rgb * 0.1964825501511404;
    color += texture(image, uv + (off1 / resolution)).rgb  * 0.2969069646728344;
    color += texture(image, uv - (off1 / resolution)).rgb  * 0.2969069646728344;
    color += texture(image, uv + (off2 / resolution)).rgb  * 0.09447039785044732;
    color += texture(image, uv - (off2 / resolution)).rgb  * 0.09447039785044732;
    color += texture(image, uv + (off3 / resolution)).rgb  * 0.010381362401148057;
    color += texture(image, uv - (off3 / resolution)).rgb  * 0.010381362401148057;
    return color;
}


void main() 
{
    fragColor.rgb = blur13(uScreenMap, TexCoords, uResolution.xy, uDirection);
    fragColor.a = texture(uScreenMap, TexCoords).a;
}