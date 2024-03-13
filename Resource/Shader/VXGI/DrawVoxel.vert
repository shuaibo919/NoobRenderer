#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;
out vec3 VoxelSpaceTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 voxelSpaceCenter;
uniform float voxelHalfSize;
uniform uint voxelResolution;

void main()
{
    vec4 wsPos = model * vec4(aPos, 1.0f);
    FragPos = wsPos.xyz;
    vec3 relativePos = wsPos.xyz - voxelSpaceCenter;
    relativePos -= relativePos.z;
    VoxelSpaceTexCoords = (relativePos / voxelHalfSize + 1.0f) * voxelResolution * 0.5f;
    gl_Position = projection * view * wsPos;
}