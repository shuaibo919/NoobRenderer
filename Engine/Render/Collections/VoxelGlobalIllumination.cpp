#include "Engine/Render/Collections/VoxelGlobalIllumination.h"
using namespace NoobRenderer::render;
void VoxelGlobalIllumination::Init(component::VoxelGlobalIllumination &gi)
{
    using namespace gtype;
    VoxelGlobalIllumination::voxelize_shader = ShaderManager::Instance().LoadShaderAndGet(
        "Resource/Shader/VXGI/Voxelize.vert", "Resource/Shader/VXGI/Voxelize.geom", "Resource/Shader/VXGI/Voxelize.frag");
    // voxel_vis_rt = std::make_shared<WriteToTexture>(gi.voxel_size, gi.voxel_size);
    gi_rt = std::make_shared<WriteToTexture>(gi.voxel_size, gi.voxel_size);
    gi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT0, gi.voxel_size, gi.voxel_size, gi.voxel_size, Format::RGBA8, GL_RGBA, DataType::UNSIGNED_BYTE); // Albedo3D
    gi_rt->GetTexture(0)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxel_size);

    gi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT1, gi.voxel_size, gi.voxel_size, gi.voxel_size, Format::RGBA8, GL_RGBA, DataType::UNSIGNED_BYTE); // Normal3D
    gi_rt->GetTexture(1)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxel_size);

    gi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT2, gi.voxel_size, gi.voxel_size, gi.voxel_size, Format::RGBA8, GL_RGBA, DataType::UNSIGNED_BYTE); // Dynamic Albedo3D
    gi_rt->GetTexture(2)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxel_size);

    gi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT3, gi.voxel_size, gi.voxel_size, gi.voxel_size, Format::RGBA8, GL_RGBA, DataType::UNSIGNED_BYTE); // Dynamic Normal3D
    gi_rt->GetTexture(3)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxel_size);

    gi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT4, gi.voxel_size, gi.voxel_size, gi.voxel_size, Format::RGBA8, GL_RGBA, DataType::UNSIGNED_BYTE); // Radiance 3D
    gi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    gi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    gi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    gi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gi_rt->GetTexture(0)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxel_size);
}
void VoxelGlobalIllumination::Voxelize(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi)
{
}
void VoxelGlobalIllumination::Apply(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi)
{
    if (!ready)
        Init(gi);
    Voxelize(scene, gi);
    //...
}