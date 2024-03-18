#version 460 core
layout(binding = 0, rgba8) uniform image3D uVoxelImage3D;
out vec4 FragColor;
in vec3 FragPos;

uniform float   uWorldSize;
uniform int     uVoxelResolution;
uniform vec3 	uVoxelCenter;
void main()
{
    const float f_dimension = float(uVoxelResolution);
	vec3 voxelCoord = ((FragPos - uVoxelCenter) / uWorldSize + 0.5f) * f_dimension;
	vec4 voxelColor = imageLoad(uVoxelImage3D, ivec3(voxelCoord));
	FragColor = vec4(voxelColor.rgb, 1.0f);
}