#include "Engine/Render/Collections/VoxelGlobalIllumination.h"
#include "Engine/Render/Collections/Utils.h"
using namespace NoobRenderer::render;
void VoxelGlobalIllumination::Init(component::VoxelGlobalIllumination &gi)
{
    using namespace gtype;
    voxelize_shader = ShaderManager::Instance().LoadShaderAndGet(
        "Resource/Shader/VXGI/Voxelize.vert", "Resource/Shader/VXGI/Voxelize.geom", "Resource/Shader/VXGI/Voxelize.frag");
    // voxel_vis_rt = std::make_shared<WriteToTexture>(gi.voxel_size, gi.voxel_size);
    gi_rt = std::make_shared<WriteToTexture>(gi.voxel_size, gi.voxel_size);
    gi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT0, gi.voxel_size, gi.voxel_size, gi.voxel_size, Format::RGBA8, Format::RGBA, DataType::UNSIGNED_BYTE); // Albedo3D
    gi_rt->GetTexture(0)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxel_size);

    gi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT1, gi.voxel_size, gi.voxel_size, gi.voxel_size, Format::RGBA8, Format::RGBA, DataType::UNSIGNED_BYTE); // Normal3D
    gi_rt->GetTexture(1)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxel_size);

    gi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT2, gi.voxel_size, gi.voxel_size, gi.voxel_size, Format::RGBA8, Format::RGBA, DataType::UNSIGNED_BYTE); // Dynamic Albedo3D
    gi_rt->GetTexture(2)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxel_size);

    gi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT3, gi.voxel_size, gi.voxel_size, gi.voxel_size, Format::RGBA8, Format::RGBA, DataType::UNSIGNED_BYTE); // Dynamic Normal3D
    gi_rt->GetTexture(3)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxel_size);

    gi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT4, gi.voxel_size, gi.voxel_size, gi.voxel_size, Format::RGBA8, Format::RGBA, DataType::UNSIGNED_BYTE); // Radiance 3D
    gi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    gi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    gi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    gi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gi_rt->GetTexture(0)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxel_size);
}
void VoxelGlobalIllumination::Voxelize(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi)
{
    float zero[] = {0.0f, 0.0f, 0.0f, 0.0f};
    gi_rt->GetTexture(0)->ClearTexImage(0, zero); // clear Albedo3D
    gi_rt->GetTexture(1)->ClearTexImage(0, zero); // clear Normal3D
    gi_rt->Bind();
    gi_rt->GetTexture(0)->BindImage(0, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI); // Albedo3D: RGBA8 as a R32UI
    gi_rt->GetTexture(1)->BindImage(1, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI); // Normal3D: RGBA8 as a R32UI
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, gi.voxel_size, gi.voxel_size);
    auto &camera = scene->GetCurrentCameraComponent();
    auto registry = scene->GetRegistry();
    auto view = registry->view<component::MeshData>();
    // voxelize_shader->SetUniform("projection", voxel_projection);
    for (auto [entity, mesh_data] : view.each())
    {
        /* Get Transform */
        glm::mat4 model = scene->GetSceneNodeByEntity(entity)->model;
        voxelize_shader->SetUniform("model", model);
        auto &mesh_ptr = mesh_data.mesh;
        if (mesh_ptr == nullptr)
            continue;

        /* Mesh Material Setting */
        auto material = std::dynamic_pointer_cast<PBRMaterial>(mesh_ptr->GetMaterial());
        Texture::ResetSlot();
        auto has_albedo = utils::SetMaterialUniform(material->albedo, voxelize_shader, "albedo", utils::MatUniValue::Vec3);
        voxelize_shader->SetUniform("hasAlbedo", static_cast<float>(has_albedo));

        /* Draw */
        mesh_ptr->Draw();
    }
    gi_rt->GetTexture(0)->Unbind();
    gi_rt->GetTexture(1)->Unbind();
    gi_rt->Unbind();
}
void VoxelGlobalIllumination::LightInjection(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi)
{
}
void VoxelGlobalIllumination::BuildMipmap(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi)
{
}
void VoxelGlobalIllumination::ConeTracing(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi)
{
}
void VoxelGlobalIllumination::Apply(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi)
{
    if (!ready)
        Init(gi);
    Voxelize(scene, gi);
    LightInjection(scene, gi);
    BuildMipmap(scene, gi);
    ConeTracing(scene, gi);
}