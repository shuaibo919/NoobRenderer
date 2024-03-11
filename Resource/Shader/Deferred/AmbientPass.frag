#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMetalRoughAo;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;  
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float hasIrradianceMap;
uniform float hasPrefilterMap;
uniform float hasBrdfLUT;

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);


void main()
{
    // Retrieve data from gbuffer: view space
    vec3 worldSpaceFragPos = texture(gPositionDepth, TexCoords).rgb;
    vec3 worldSpaceNormal = texture(gNormal, TexCoords).rgb;
    vec3 albedo =  pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2));
    float metallic =  texture(gMetalRoughAo, TexCoords).r;
    float roughness = texture(gMetalRoughAo, TexCoords).g;
    
    vec3 N = worldSpaceNormal;
    vec3 V = normalize(viewPos - worldSpaceFragPos);
    vec3 R = reflect(-V,N);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	

    // replace this ambient lighting with environment lighting).
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = ((1. - hasIrradianceMap) * vec3(1.0f) + hasIrradianceMap * irradiance ) * albedo;
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = hasBrdfLUT * hasPrefilterMap * prefilteredColor * (F * brdf.x + brdf.y);

    FragColor = vec4(ambientStrength * (kD * diffuse + specular), 1.0);
}


vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

