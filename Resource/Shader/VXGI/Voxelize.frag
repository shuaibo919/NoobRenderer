#version 460 core
#extension GL_ARB_shader_image_load_store : require
layout(binding = 0, r32ui) uniform volatile coherent uimage3D uVoxelTexture;

in FragData {
	vec3 WsPos;
	vec3 Normal;
	vec2 TexCoord;
	flat int axis;
}fs_in;

struct Material {
    vec3 albedo;
}; 

struct MaterialMap {
   sampler2D TextureAlbedo;
};

uniform Material material;
uniform MaterialMap materialMap;
uniform float uAlbedoTexEnabled;
uniform float   uWorldSize;
uniform int     uVoxelResolution;
uniform vec3 	uVoxelCenter;
// out vec4 FragColor;

ivec3 GetVoxelCoord();

void main()
{
	ivec3 voxelPos = GetVoxelCoord();
	vec3  gAlbedo  = (1.f - uAlbedoTexEnabled) * material.albedo + uAlbedoTexEnabled * texture(materialMap.TextureAlbedo, fs_in.TexCoord).rgb;

    // imageAtomicRGBA8Avg(uVoxelTexture, voxelPos, vec4(gAlbedo,1.0));

	vec3 val = gAlbedo;
	uint newVal = packUnorm4x8(vec4(val, 1.0f / 255.0f));
    uint prevStoredVal = 0;
    uint curStoredVal;

    // Loop as long as destination value gets changed by other threads
    while((curStoredVal = imageAtomicCompSwap(uVoxelTexture, voxelPos, prevStoredVal, newVal))!= prevStoredVal)
    {
        prevStoredVal = curStoredVal;
        vec4 unpacked = unpackUnorm4x8(curStoredVal);
        vec3 x = unpacked.xyz;

        float n = unpacked.w * 255.0f;

        x = (x * n + val.xyz) / (n + 1);
        newVal = packUnorm4x8(vec4(x, (n + 1) / 255.0f));
    }
}
ivec3 GetVoxelCoord()
{
	vec3 location = ((fs_in.WsPos - uVoxelCenter) / uWorldSize + 0.5f) * float(uVoxelResolution);
	return ivec3(location);
}
