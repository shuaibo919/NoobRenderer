#version 460 core
layout (location = 0) out vec4 gPositionDepth;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gMetalRoughAo;
layout (location = 4) out vec3 gEmission;
layout (location = 5) out float gStencil;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;


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
 
/* Uniform Material */
uniform Material material;
uniform MaterialMap materialMap;
uniform float hasAlbedo;
uniform float hasMetallic;
uniform float hasRoughness;
uniform float hasNormalMap;
uniform float hasEmission;
uniform bool hasHeightMap;


vec2 ApplyParallaxMapping(vec3 pViewDir, vec2 pTexCoords);
vec3 ApplyNormalMapping(vec3 pFragPos, vec3 pNormal, vec2 pTexCoords);


void main()
{        
    gPositionDepth.rgb = FragPos;
    gPositionDepth.a = gl_FragCoord.z;

    gStencil.r = 1;

    // todo:Appling parallax mapping
    vec2 texCoord = TexCoords;

    gNormal = (1.f - hasNormalMap) * Normal + hasNormalMap * ApplyNormalMapping(gPositionDepth.rgb, Normal, texCoord);
    gAlbedo = (1.f - hasAlbedo) * material.albedo + hasAlbedo * texture(materialMap.TextureAlbedo, texCoord).rgb;
    
    float metallic =  (1.f - hasMetallic) * material.metallic + hasMetallic * texture(materialMap.TextureMetallic, texCoord).r;
    float roughness =   (1.f - hasRoughness) * material.roughness + hasRoughness * texture(materialMap.TextureRoughness, texCoord).r;
    gMetalRoughAo = vec3(metallic, roughness, 0.f);

    gEmission = (1.f - hasEmission) * material.emission + hasEmission * vec3(texture(materialMap.TextureEmission, texCoord));
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