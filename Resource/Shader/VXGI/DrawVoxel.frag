#version 460 core
layout(binding = 0, rgba8) uniform readonly image3D voxelImage;
in vec3 FragPos;
in vec3 VoxelSpaceTexCoords;
out vec4 color;
void main()
{
    albedo = imageLoad(voxelImage, VoxelSpaceTexCoords);
}