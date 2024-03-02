#version 460 core
out vec4 uLightColor;
#define MAX_LIGHTS 20
#define MAX_POINT_LIGHT_SHADOW_MAP 2
const float PI = 3.14159265359;

struct PointLight {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;

    float far;
    int castShadow;
};

struct DirLight {
    vec3 direction;
    vec3 color;
    // shadow setting
    sampler2DArray shadowMap;
    float castShadow;
    float intensity;
    int cascadeCount; 
    float biasFactor;
    float minBias;
    float enable;
    float shadowRadius;
}; 

in vec2 TexCoords;

/* G-Buffers */
uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMetalRoughAo;
uniform sampler2D gEmission;

/* G-Buffers */
uniform vec3 viewPos;
uniform mat4 view;
uniform vec2 viewportSize;

/* Lights and Shadows */
uniform PointLight pointLights[MAX_LIGHTS];
uniform DirLight mainDirLight;
uniform int nPointLights;
uniform samplerCube pointLightShadowMap[MAX_POINT_LIGHT_SHADOW_MAP];
uniform float cascadePlaneDistances[12];
uniform float cascadeBiasModifier[12];
layout (std140, binding = 1) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[12];
};

/* Function of Direct Lighting(PBR) */
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
vec3 DirectionalLightCalc(DirLight pDirLight, vec3 pFragPos, vec3 pNormal, vec3 pView, vec3 pF0, vec3 pAlbedo, float pRoughness, float pMetallic);
vec3 PointLightCalc(PointLight pPointLight, vec3 pFragPos, vec3 pNormal, vec3 pView, vec3 pF0, vec3 pAlbedo, float pRoughness, float pMetallic);



void main()
{
    // Retrieve data from gbuffer: world space
    vec3 worldSpaceFragPos = texture(gPositionDepth, TexCoords).rgb;
    vec3 worldSpaceNormal = texture(gNormal, TexCoords).rgb;
    vec3 albedo =  pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2));
    float metallic =  texture(gMetalRoughAo, TexCoords).r;
    float roughness = texture(gMetalRoughAo, TexCoords).g;
    float ao = texture(gMetalRoughAo, TexCoords).b;
    vec3 emission = texture(gEmission, TexCoords).rgb;
    
    // Calculate common used values
    vec3 viewDir = normalize(viewPos - worldSpaceFragPos);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // Lighting
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < nPointLights; ++i) 
    {
        Lo += PointLightCalc(pointLights[i], worldSpaceFragPos, worldSpaceNormal, viewDir, F0, albedo, roughness, metallic);
    }
    Lo += DirectionalLightCalc(mainDirLight, worldSpaceFragPos, worldSpaceNormal, viewDir, F0, albedo, roughness, metallic);

    // Frag Color
    float transparency = 1.0f;
    vec3 color = Lo + emission;
    uLightColor = vec4(color, transparency);
}


float DistributionGGX(vec3 N, vec3 H, float roughness) 
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) 
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float denom = NdotV * (1.0 - k) + k;
    return NdotV / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) 
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) 
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

float PCFApproximation(sampler2DArray shadowMap, vec2 projCoords, float shadowRadius, int layer, float cur_depth)
{
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
	float dx0 = - texelSize.x * shadowRadius;
	float dy0 = - texelSize.y * shadowRadius;
	float dx1 = + texelSize.x * shadowRadius;
	float dy1 = + texelSize.y * shadowRadius;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords  + vec2( dx0, dy0 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords  + vec2( 0.0, dy0 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords  + vec2( dx1, dy0 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords  + vec2( dx0, 0.0 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords  + vec2( dx1, 0.0 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords  + vec2( 0.0, dy1 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords  + vec2( dx0, dy1 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords  + vec2( dx1, dy1 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords                    , layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords + vec2(dx0*0.5, dy0*0.5 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords + vec2(0.0    , dy0*0.5 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords + vec2(dx1*0.5, dy0*0.5 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords + vec2(dx0*0.5, 0.0     ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords + vec2(dx1*0.5, 0.0     ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords + vec2(dx0*0.5, dy1*0.5 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords + vec2(0.0    , dy1*0.5 ), layer)).r ? 1.0 : 0.0;
    shadow += cur_depth > texture(shadowMap, vec3(projCoords + vec2(dx1*0.5, dy1*0.5 ), layer)).r ? 1.0 : 0.0;
    return shadow / 17.0;
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

float CascadeShadowCalc(DirLight light, vec3 pFragPos, vec3 pFragNormal, int layer)
{
    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(pFragPos, 1.0);
    vec3 shadowCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;  // perform perspective divide
    float currentDepth = shadowCoords.z;                              // get depth of current fragment from light's perspective

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(pFragNormal);
    float bias = max(light.biasFactor * (1.0 - dot(normal, normalize(light.direction))), light.minBias);
    bias *= 1 / cascadePlaneDistances[layer] * cascadeBiasModifier[layer];

    // PCF    
    float cur_depth = currentDepth - bias;
    float shadow = PCFApproximation(light.shadowMap, shadowCoords.xy, light.shadowRadius, layer, cur_depth);
    
    return shadow * light.castShadow;
}


vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float PointShadowCalc(PointLight light, vec3 pFragPos)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = pFragPos -light.position;
    float currentDepth = length(fragToLight); 

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - pFragPos);
    float diskRadius = (1.0 + (viewDistance / light.far)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(pointLightShadowMap[light.castShadow], fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= light.far;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

vec3 DirectionalLightCalc(DirLight pDirLight, vec3 pFragPos, vec3 pNormal, vec3 pView, vec3 pF0, vec3 pAlbedo, float pRoughness, float pMetallic)
{

    vec3 L = normalize(pDirLight.direction); // In world Space
    vec3 H = normalize(pView + L);        
    vec3 radiance = pDirLight.color * pDirLight.intensity;

    // Cook-Torrance BRDF
    float NDF  = DistributionGGX(pNormal, H, pRoughness);   
    float G    = GeometrySmith(pNormal, pView, L, pRoughness);      
    vec3  F    = FresnelSchlick(max(dot(H, pView), 0.0), pF0);

    float NdotL = max(dot(pNormal, L), 0.0);
    float NdotV = max(dot(pNormal, pView), 0.0);

    vec3 specular = (NDF * G * F) / max(4.0 * NdotV * NdotL, 0.0001); // + 0.0001 to prevent divide by zero
        
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't be above 1.0 (unless the surface emits light); to preserve this relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals have diffuse lighting, or a linear blend if partly metal (pure metals have no pMetallic light).
    kD *= (1.0 - pMetallic);	 

    int layer = GetCascadeLayer(pDirLight, pFragPos);
    float shadow = CascadeShadowCalc(pDirLight, pFragPos, pNormal, layer); // add shadow

    return  pDirLight.enable * (kD * pAlbedo / PI + specular) * radiance * NdotL * (1 - shadow) ; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}

vec3 PointLightCalc(PointLight pPointLight, vec3 pFragPos, vec3 pNormal, vec3 pView, vec3 pF0, vec3 pAlbedo, float pRoughness, float pMetallic)
{
    vec3 L = normalize(pPointLight.position - pFragPos); // In world Space
    vec3 H = normalize(pView + L);
        
    float ligh_distance = length(pPointLight.position - pFragPos);
    float attenuation = 1.0 / (pPointLight.constant + pPointLight.linear * ligh_distance + pPointLight.quadratic * (ligh_distance * ligh_distance));
    vec3 radiance = pPointLight.color * attenuation;
        
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(pNormal, H, pRoughness);   
    float G   = GeometrySmith(pNormal, pView, L, pRoughness);      
    vec3 F    = FresnelSchlick(max(dot(H, pView), 0.0), pF0);
           
    vec3 numerator    = NDF * G * F; 
    float denominator = 4.0 * max(dot(pNormal, pView), 0.0) * max(dot(pNormal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
        
    vec3 kS = F; 
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - pMetallic;	  

    float NdotL = max(dot(pNormal, L), 0.0);         // scale light by NdotL
    float shadow = 0.0f;
    if(pPointLight.castShadow != -1)
    {
        shadow = PointShadowCalc(pPointLight, pFragPos); // add shadow 
    }

    // add to outgoing radiance Lo
    return  mix((kD * pAlbedo / PI + specular) * radiance * NdotL, vec3(0.0f), shadow);  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}
