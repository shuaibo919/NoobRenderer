#version 460 core
out vec3 uVolumetricLightColor;
const float PI = 3.14159265359;

struct DirLight {
    vec3 direction;
    vec3 color;
    // shadow setting
    sampler2DArray shadowMap;
    float castShadow;
    float enable;
    int cascadeCount; 
    // volumetric
    int vStep;
    float vScatter;
    float vIntensity;
}; 

in vec2 TexCoords;

/* G-Buffers */
uniform sampler2D gPositionDepth;

uniform vec3 viewPos;
uniform mat4 view;
uniform vec2 viewportSize;

/* Lights and Shadows */
uniform DirLight mainDirLight;
uniform float cascadePlaneDistances[12];
layout (std140, binding = 1) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[12];
};

int GetCascadeLayer(DirLight light, vec3 pFragPos);
float ScatteringCalc(float G, float cosTheta);

const mat4 DitherPattern = mat4(
      vec4(0.0f, 0.5f, 0.125f, 0.625f),
      vec4(0.75f, 0.22f, 0.875f, 0.375f),
      vec4(0.1875f, 0.6875f, 0.0625f, 0.5625f),
      vec4(0.9375f, 0.4375f, 0.8125f, 0.3125f)
);


void main()
{
    /* Retrieve data from gbuffer */
    vec3 worldSpaceFragPos = texture(gPositionDepth, TexCoords).rgb;
    float float_num_steps = float(mainDirLight.vStep);
    vec3 viewPosToFragPosVec = worldSpaceFragPos - viewPos;
    float stepSize = length(viewPosToFragPosVec) / float_num_steps;
    viewPosToFragPosVec = normalize(viewPosToFragPosVec);
    vec3 marchingStep = viewPosToFragPosVec * stepSize;
    vec3 volumetric = vec3(0.0f);
    vec2 screen_size = textureSize(gPositionDepth, 0);
    vec3 marching_position = viewPos + marchingStep * DitherPattern[int(TexCoords.x * viewportSize.x) % 4][int(TexCoords.y * viewportSize.y) % 4];
    vec3 norm_dir = normalize(mainDirLight.direction);
    for (int i = 0; i < mainDirLight.vStep; ++i)
    {
        int layer = GetCascadeLayer(mainDirLight, marching_position);
        vec4 lightSpacePos = lightSpaceMatrices[layer] * vec4(marching_position, 1.0f);
        vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w  * 0.5 + 0.5;  
        float depth = texture(mainDirLight.shadowMap, vec3(projCoords.xy, layer)).r;
        if (depth > projCoords.z)
        {        
            volumetric += ScatteringCalc(mainDirLight.vScatter, dot(viewPosToFragPosVec, norm_dir)) * mainDirLight.color;
        }
        marching_position += marchingStep;
    }
    uVolumetricLightColor =  mainDirLight.enable * mainDirLight.vIntensity * clamp(volumetric / float_num_steps, 0.0f, 1.0f);
}



int GetCascadeLayer(DirLight light, vec3 pFragPos)
{
    vec4 fragPosViewSpace = view * vec4(pFragPos, 1.0); 
    float depthValue = abs(fragPosViewSpace.z);
    int layer = light.cascadeCount - 1;
    for (int i = 0; i < light.cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    return layer;
}

float ScatteringCalc(float G, float cosTheta)
{
    // Mie-scattering phase function approximated by the Henyey-Greenstein phase function
    return (1.0f - G * G) / (4.0f * PI * pow(1.0f + G * G - (2.0f * G * cosTheta), 1.5f));
}
