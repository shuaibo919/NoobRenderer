#version 460 core

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    vec3 emission;
}; 

struct MaterialMap {
   sampler2D TextureAlbedo;
   sampler2D TextureMetallic;
   sampler2D TextureRoughness;
   sampler2D TextureNormal;
   sampler2D TextureHeight;
   sampler2D TextureEmission;
};

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

out vec4 FragColor;

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

const float PI = 3.14159265359;
float heightScale = 0.1;

vec2 ApplyParallaxMapping(vec3 pViewDir, vec2 pTexCoords);
vec3 ApplyNormalMapping(vec3 pFragPos, vec3 pNormal, vec2 pTexCoords);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0); 
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

void main() 
{
    vec3 worldSpaceFragPos = FragPos;
    vec3 mNormal = (1.f - hasNormalMap) * Normal + hasNormalMap * ApplyNormalMapping(worldSpaceFragPos, Normal, TexCoords);
    vec3 mAlbedo = (1.f - hasAlbedo) * material.albedo + hasAlbedo * texture(materialMap.TextureAlbedo, TexCoords).rgb;
    vec3 mEmission = (1.f - hasEmission) * material.emission + hasEmission * vec3(texture(materialMap.TextureEmission, TexCoords));
    float metallic =  (1.f - hasMetallic) * material.metallic + hasMetallic * texture(materialMap.TextureMetallic, TexCoords).r;
    float roughness =   (1.f - hasRoughness) * material.roughness + hasRoughness * texture(materialMap.TextureRoughness, TexCoords).r;
   
    vec3 viewDir = normalize(viewPos - worldSpaceFragPos);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, mAlbedo, metallic);
    
    vec3 Lo = vec3(0.0);
    vec3 position[2] = vec3[2](vec3(viewPos), vec3(-viewPos.x,viewPos.y,viewPos.z));
    for(int i = 0; i < 2; ++i) 
    {
        vec3 L = normalize(position[i]- worldSpaceFragPos);
        vec3 H = normalize(viewDir + L);
        float ligh_distance = length(position[i] - worldSpaceFragPos);
        float attenuation = 1.0 / (1.0f + 0.009f * ligh_distance + 0.0032f * (ligh_distance * ligh_distance));
        vec3 radiance = vec3(1.0f) * attenuation;
        float NDF = DistributionGGX(mNormal, H, roughness);   
        float G   = GeometrySmith(mNormal, viewDir, L, roughness);      
        vec3 F    = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);
        float denominator = 4.0 * max(dot(mNormal, viewDir), 0.0) * max(dot(mNormal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = NDF * G * F / denominator;
        vec3 kD = (vec3(1.0) - F) * (1.0 - metallic); 
        float NdotL = max(dot(mNormal, L), 0.0);        

        Lo += (kD * mAlbedo / PI + specular) * radiance * NdotL; 
    }
    FragColor = vec4(Lo + mEmission, 1.0f);
    
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

vec2 ApplyParallaxMapping(vec3 pViewDir, vec2 pTexCoords)
{
    return vec2(0.0f);
}
vec3 ApplyNormalMapping(vec3 pFragPos, vec3 pNormal, vec2 pTexCoords)
{
    vec3 tangentNormal = texture(materialMap.TextureNormal, pTexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(pFragPos);
    vec3 Q2  = dFdy(pFragPos);
    vec2 st1 = dFdx(pTexCoords);
    vec2 st2 = dFdy(pTexCoords);

    vec3 N   = normalize(pNormal);
    vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}