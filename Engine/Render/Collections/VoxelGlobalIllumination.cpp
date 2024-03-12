#include "Engine/Render/Collections/VoxelGlobalIllumination.h"
#include "Engine/Render/Collections/Utils.h"
using namespace NoobRenderer::render;
void VoxelGlobalIllumination::Init(component::VoxelGlobalIllumination &gi)
{
    using namespace Texture;
    voxelize_shader = ShaderManager::Instance().LoadShaderAndGet(
        "Resource/Shader/VXGI/Voxelize.vert", "Resource/Shader/VXGI/Voxelize.geom", "Resource/Shader/VXGI/Voxelize.frag");
    // voxel_vis_rt = std::make_shared<WriteToTexture>(gi.voxelize_resolution, gi.voxelize_resolution);
    vxgi_rt = std::make_shared<WriteToTexture>(gi.voxelize_resolution, gi.voxelize_resolution);
    vxgi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT0, gi.voxelize_resolution, gi.voxelize_resolution, gi.voxelize_resolution, Format::RGBA8, Format::RGBA, DataType::UNSIGNED_BYTE); // Albedo3D
    vxgi_rt->GetTexture(0)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxelize_resolution);

    vxgi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT1, gi.voxelize_resolution, gi.voxelize_resolution, gi.voxelize_resolution, Format::RGBA8, Format::RGBA, DataType::UNSIGNED_BYTE); // Normal3D
    vxgi_rt->GetTexture(1)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxelize_resolution);

    vxgi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT2, gi.voxelize_resolution, gi.voxelize_resolution, gi.voxelize_resolution, Format::RGBA8, Format::RGBA, DataType::UNSIGNED_BYTE); // Dynamic Albedo3D
    vxgi_rt->GetTexture(2)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxelize_resolution);

    vxgi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT3, gi.voxelize_resolution, gi.voxelize_resolution, gi.voxelize_resolution, Format::RGBA8, Format::RGBA, DataType::UNSIGNED_BYTE); // Dynamic Normal3D
    vxgi_rt->GetTexture(3)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxelize_resolution);

    vxgi_rt->SetTexture3D<Texture3D>(GL_COLOR_ATTACHMENT4, gi.voxelize_resolution, gi.voxelize_resolution, gi.voxelize_resolution, Format::RGBA8, Format::RGBA, DataType::UNSIGNED_BYTE); // Radiance 3D
    vxgi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    vxgi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    vxgi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    vxgi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    vxgi_rt->GetTexture(4)->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    vxgi_rt->GetTexture(0)->TexStorage3D(GL_TEXTURE_3D, 1, gi.voxelize_resolution);
}
void VoxelGlobalIllumination::Voxelize(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi)
{
    float zero[] = {0.0f, 0.0f, 0.0f, 0.0f};
    vxgi_rt->GetTexture(0)->ClearTexImage(0, zero); // clear Albedo3D
    vxgi_rt->GetTexture(1)->ClearTexImage(0, zero); // clear Normal3D
    vxgi_rt->Bind();
    vxgi_rt->GetTexture(0)->BindImage(0, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI); // Albedo3D: RGBA8 as a R32UI
    vxgi_rt->GetTexture(1)->BindImage(1, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI); // Normal3D: RGBA8 as a R32UI
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, gi.voxelize_resolution, gi.voxelize_resolution);
    auto &camera = scene->GetCurrentCameraComponent();
    auto registry = scene->GetRegistry();
    auto view = registry->view<component::MeshData>();

    auto projection = glm::ortho(-gi.half_size, gi.half_size, -gi.half_size, gi.half_size, 0.0f, 2.0f * gi.half_size);
    voxel_projection_view_mat[0] = glm::lookAt(gi.voxelize_center + glm::vec3(gi.half_size, 0.0f, 0.0f), gi.voxelize_center, glm::vec3(0.0f, 1.0f, 0.0f));
    voxel_projection_view_mat[1] = glm::lookAt(gi.voxelize_center + glm::vec3(0.0f, gi.half_size, 0.0f), gi.voxelize_center, glm::vec3(0.0f, 0.0f, -1.0f));
    voxel_projection_view_mat[2] = glm::lookAt(gi.voxelize_center + glm::vec3(0.0f, 0.0f, gi.half_size), gi.voxelize_center, glm::vec3(0.0f, 1.0f, 0.0f));
    for (int i = 0; i < 3; i++)
    {
        auto number = std::to_string(i);
        voxel_projection_view_mat[i] = projection * voxel_projection_view_mat[i];
        voxelize_shader->SetUniform("voxelPVMat[" + number + "]", voxel_projection_view_mat[i]);
    }
    voxelize_shader->SetUniform("voxelResolution", gi.voxelize_resolution);
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
    vxgi_rt->GetTexture(0)->Unbind();
    vxgi_rt->GetTexture(1)->Unbind();
    vxgi_rt->Unbind();
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