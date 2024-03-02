#version 460 core
layout (location = 0) out float lMetalic;
layout (location = 1) out float lRoughness;

in vec2 TexCoords;
uniform sampler2D MetallicRoughnessImage;

void main()
{        
    // GLTF
    lMetalic = texture(MetallicRoughnessImage, TexCoords).r;
    lRoughness =  texture(MetallicRoughnessImage, TexCoords).g;
}
