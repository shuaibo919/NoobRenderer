#version 460 core

#define MAX_LIGHTS 4
#define MAX_POINT_LIGHT_SHADOW_MAP 2


struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    vec3 emission;
}; 

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
    float cascadePlaneDistances[12];
    int cascadeCount; 
    float biasFactor;
    float minBias;
    float biasModifier;
    float enable;
}; 

layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[12];
};

struct MaterialMap {
   sampler2D TextureAlbedo;
   sampler2D TextureMetallic;
   sampler2D TextureRoughness;
   sampler2D TextureNormal;
   sampler2D TextureHeight;
   sampler2D TextureAmbientOcclusion;
   sampler2D TextureEmission;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
uniform mat4 view;

out vec4 FragColor;

uniform PointLight pointLights[MAX_LIGHTS];
uniform DirLight mainDirLight;
uniform int nPointLights;
uniform samplerCube pointLightShadowMap[MAX_POINT_LIGHT_SHADOW_MAP];

uniform Material material;
uniform MaterialMap materialMap;
uniform float hasAlbedo;
uniform float hasMetallic;
uniform float hasRoughness;
uniform float hasNormalMap;
uniform float hasAmbientOcclusion;
uniform float hasEmission;
uniform bool hasHeightMap;

uniform vec3 viewPos;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;  

uniform bool hasIrradianceMap;
uniform bool hasPrefilterMap;
uniform bool hasBrdfLUT;


const float PI = 3.14159265359;
float heightScale = 0.1;

vec2 texCoord;
vec3 GetNormalFromMap();
vec2 ParallaxMapping(vec2 texCoords);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
vec3 AccumulateDirLight(DirLight pDirLight, vec3 pNormal, vec3 pView, vec3 pF0, vec3 pAlbedo, float pRoughness, float pMetallic);
vec3 AcuumulatePointLight(PointLight pPointLight, vec3 pNormal, vec3 pView,vec3 pF0,vec3 pAlbedo,  float pRoughness, float pMetallic);


void main() {

    // Paralax mapping
    texCoord = TexCoord;
    // texCoord = ParallaxMapping(TexCoord);
    // if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
    //     discard;

    // Material
    vec3 albedo = (1.f - hasAlbedo) * material.albedo + hasAlbedo * pow(texture(materialMap.TextureAlbedo, texCoord).rgb, vec3(2.2));
    float metallic =  (1.f - hasMetallic) * material.metallic + hasMetallic * texture(materialMap.TextureMetallic, texCoord).r;
    float roughness =   (1.f - hasRoughness) * material.roughness + hasRoughness * texture(materialMap.TextureRoughness, texCoord).r;
    float ao =  (1.f - hasAmbientOcclusion) * material.ao + hasAmbientOcclusion * texture(materialMap.TextureAmbientOcclusion, texCoord).r;
    vec3 emissionLighting =   (1.f - hasEmission) * material.emission + hasEmission * vec3(texture(materialMap.TextureEmission, texCoord));

    // Normal, View, Projection
    vec3 N = (1.f - hasNormalMap) * Normal + hasNormalMap * GetNormalFromMap();
    vec3 V = normalize(viewPos - FragPos);
    vec3 R = reflect(-V,N);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 directLighting = vec3(0.0);
    for(int i = 0; i < nPointLights; ++i) directLighting += AcuumulatePointLight(pointLights[i], N, V, F0, albedo, roughness, metallic);
    directLighting += AccumulateDirLight(mainDirLight, N, V, F0, albedo, roughness, metallic);

    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	
    
    // replace this ambient lighting with environment lighting).
    vec3 diffuse = vec3(0.2) * albedo;
    if(hasIrradianceMap){
        vec3 irradiance = texture(irradianceMap, N).rgb;
        diffuse      = irradiance * albedo;
    }

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    vec3 specular = vec3(0.0);
    const float MAX_REFLECTION_LOD = 4.0;
    if(hasBrdfLUT){
        if(hasPrefilterMap){
            vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
            vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
            specular = prefilteredColor * (F * brdf.x + brdf.y);
        }
    }

    vec3 indirectLighting = kD * (diffuse + specular) * ao;
    vec3 color = indirectLighting + emissionLighting + directLighting;

    // HDR tonemapping
    color = color / (color + vec3(1.0));

    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    // Apply transparency
    float transparency = (1.f - hasAlbedo) + hasAlbedo * texture(materialMap.TextureAlbedo, texCoord).a;

    // Color
    FragColor = vec4(color, transparency);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) 
{

    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) 
{

    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) 
{

    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) 
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   


vec3 GetNormalFromMap() 
{

    vec3 tangentNormal = texture(materialMap.TextureNormal, texCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(texCoord);
    vec2 st2 = dFdy(texCoord);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec2 ParallaxMapping(vec2 texCoords) { 

    vec2 uvs = texCoords;

    if(hasHeightMap) {

        // Calculate viewDir 
        vec3 Q1  = dFdx(FragPos);
        vec3 Q2  = dFdy(FragPos);
        vec2 st1 = dFdx(texCoord);
        vec2 st2 = dFdy(texCoord);

        vec3 N   = normalize(Normal);
        vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
        vec3 B  = -normalize(cross(N, T));
        mat3 TBN = mat3(T, B, N);

        vec3 tangentViewPos  = TBN * viewPos;
        vec3 tangentFragPos  = TBN * FragPos;

        vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
        
        // Paralax mapping
        float height = texture(materialMap.TextureHeight, texCoords).r;
        vec2 offset = viewDir.xy * (height * heightScale);
        return texCoords - offset; 
    }
 
    return uvs;
}


float CascadeShadowCalculation(DirLight light, vec3 pFragPos, vec3 pFragNormal)
{
   // select cascade layer
    vec4 fragPosViewSpace = view * vec4(pFragPos, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = light.cascadeCount - 1;
    for (int i = 0; i < light.cascadeCount; ++i)
    {
        if (depthValue < light.cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(pFragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;  // perform perspective divide
    projCoords = projCoords * 0.5 + 0.5;                            // transform to [0,1] range
    float currentDepth = projCoords.z;                              // get depth of current fragment from light's perspective

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(pFragNormal);
    float bias = max(light.biasFactor * (1.0 - dot(normal, normalize(light.direction))), light.minBias);
    bias *= 1 / (light.cascadePlaneDistances[layer]) * light.biasModifier;

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(light.shadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(light.shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
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

float PointShadowCalculation(PointLight light, vec3 pFragPos, vec3 pFragNormal)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = pFragPos - light.position;
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

    return shadow * light.castShadow;
}


vec3 AccumulateDirLight(DirLight pDirLight, vec3 pNormal, vec3 pView, vec3 pF0, vec3 pAlbedo, float pRoughness, float pMetallic)
{
   // calculate per-light radiance
   vec3 L = normalize(pDirLight.direction);
   vec3 H = normalize(pView + L);
        
   vec3 radiance = pDirLight.color * pDirLight.intensity;

   // Cook-Torrance BRDF
   float NDF  = DistributionGGX(pNormal, H, pRoughness);   
   float G    = GeometrySmith(pNormal, pView, L, pRoughness);      
   vec3  F    = FresnelSchlick(max(dot(H, pView), 0.0), pF0);
           
   vec3 numerator    = NDF * G * F; 
   float denominator = 4.0 * max(dot(pNormal, pView), 0.0) * max(dot(pNormal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
   vec3 specular = numerator / denominator;
        
   // kS is equal to Fresnel
   vec3 kS = F;
   // for energy conservation, the diffuse and specular light can't
   // be above 1.0 (unless the surface emits light); to preserve this
   // relationship the diffuse component (kD) should equal 1.0 - kS.
   vec3 kD = vec3(1.0) - kS;
   // multiply kD by the inverse metalness such that only non-metals 
   // have diffuse lighting, or a linear blend if partly metal (pure metals
   // have no pMetallic light).
   kD *= 1.0 - pMetallic;	  

   // scale light by NdotL
   float NdotL = max(dot(pNormal, L), 0.0);        

   // add shadow
   float shadow = CascadeShadowCalculation(pDirLight, FragPos, pNormal);

   return  pDirLight.enable * mix((kD * pAlbedo / PI + specular) * radiance * NdotL, vec3(0.0f), shadow);  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    
}

vec3 AcuumulatePointLight(PointLight pPointLight, vec3 pNormal, vec3 pView, vec3 pF0, vec3 pAlbedo, float pRoughness, float pMetallic)
{
   // calculate per-light radiance
   vec3 L = normalize(pPointLight.position - FragPos);
   vec3 H = normalize(pView + L);
        
   float distance = length(pPointLight.position - FragPos);
   float attenuation = 1.0 / (pPointLight.constant + pPointLight.linear * distance + pPointLight.quadratic * (distance * distance));
   vec3 radiance = pPointLight.color * attenuation;
        

   // Cook-Torrance BRDF
   float NDF = DistributionGGX(pNormal, H, pRoughness);   
   float G   = GeometrySmith(pNormal, pView, L, pRoughness);      
   vec3 F    = FresnelSchlick(max(dot(H, pView), 0.0), pF0);
           
   vec3 numerator    = NDF * G * F; 
   float denominator = 4.0 * max(dot(pNormal, pView), 0.0) * max(dot(pNormal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
   vec3 specular = numerator / denominator;
        
   // kS is equal to Fresnel
   vec3 kS = F;
   // for energy conservation, the diffuse and specular light can't
   // be above 1.0 (unless the surface emits light); to preserve this
   // relationship the diffuse component (kD) should equal 1.0 - kS.
   vec3 kD = vec3(1.0) - kS;
   // multiply kD by the inverse metalness such that only non-metals 
   // have diffuse lighting, or a linear blend if partly metal (pure metals
   // have no pMetallic light).
   kD *= 1.0 - pMetallic;	  

   // scale light by NdotL
   float NdotL = max(dot(pNormal, L), 0.0);        


    // add shadow 
    float shadow = 0.0f;
    if(pPointLight.castShadow != -1)
    {
        shadow = PointShadowCalculation(pPointLight, FragPos, pNormal); // add shadow 
    }


   // add to outgoing radiance Lo
   return   mix((kD * pAlbedo / PI + specular) * radiance * NdotL, vec3(0.0f), shadow);  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    
}
