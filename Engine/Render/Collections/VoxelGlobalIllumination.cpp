#include "Engine/Render/Collections/VoxelGlobalIllumination.h"
#include "Engine/Render/Collections/Utils.h"
#include "Engine/Function/Utility/Helpers.h"
using namespace NoobRenderer::render;
void VoxelGlobalIllumination::Init(component::VoxelGlobalIllumination &gi)
{
    ready = true;
    using namespace Texture;
    voxelize_shader = ShaderManager::Instance().LoadShaderAndGet(
        "Resource/Shader/VXGI/Voxelize.vert", "Resource/Shader/VXGI/Voxelize.frag", "Resource/Shader/VXGI/Voxelize.geom");
    GLfloat border[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    voxel_albedo = std::make_shared<Texture3D>(gi.voxelize_resolution, gi.voxelize_resolution, gi.voxelize_resolution, Format::RGBA, Format::RGBA8, DataType::UNSIGNED_BYTE);
    voxel_albedo->SetParameterAndSave(GL_TEXTURE_BORDER_COLOR, &border[0]);
    voxel_albedo->SetParameterAndSave(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    voxel_albedo->SetParameterAndSave(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    voxel_albedo->SetParameterAndSave(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    voxel_albedo->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    voxel_albedo->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    voxel_albedo->Unbind();

    vxgi_rt = std::make_shared<WriteToTexture>(gi.voxelize_resolution, gi.voxelize_resolution);
    vxgi_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, gi.voxelize_resolution, gi.voxelize_resolution, Format::RGBA, Format::RGBA16, DataType::FLOAT);
    vxgi_rt->GetTexture(0)->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    vxgi_rt->GetTexture(0)->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    vxgi_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, gi.voxelize_resolution, gi.voxelize_resolution, GL_DEPTH_COMPONENT);
    std::cout << "VoxelGlobalIllumination RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
    vxgi_rt->Unbind();

}
void VoxelGlobalIllumination::Voxelize(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi)
{
    auto projection = glm::ortho(-gi.world_size * 0.5f, gi.world_size * 0.5f, -gi.world_size * 0.5f, gi.world_size * 0.5f, 0.f, gi.world_size);
    voxel_projection_view_mat[0] = projection * glm::lookAt(gi.voxelize_center + glm::vec3(gi.world_size * 0.5f, 0.0f, 0.0f), gi.voxelize_center, glm::vec3(0.0f, 1.0f, 0.0f));
    voxel_projection_view_mat[1] = projection * glm::lookAt(gi.voxelize_center + glm::vec3(0.0f, gi.world_size * 0.5f, 0.0f), gi.voxelize_center, glm::vec3(0.0f, 0.0f, -1.0f));
    voxel_projection_view_mat[2] = projection * glm::lookAt(gi.voxelize_center + glm::vec3(0.0f, 0.0f, gi.world_size * 0.5f), gi.voxelize_center, glm::vec3(0.0f, 1.0f, 0.0f));
    vxgi_rt->Bind();
    voxel_albedo->BindImage(0, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI); // Albedo3D: RGBA8 as a R32UI
    glViewport(0, 0, gi.voxelize_resolution, gi.voxelize_resolution);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto registry = scene->GetRegistry();
    auto &camera = scene->GetCurrentCameraComponent();
    auto view = registry->view<component::MeshData>();
    voxelize_shader->SetUniform("uProjx", voxel_projection_view_mat[0]);
    voxelize_shader->SetUniform("uProjy", voxel_projection_view_mat[1]);
    voxelize_shader->SetUniform("uProjz", voxel_projection_view_mat[2]);
    voxelize_shader->SetUniform("uVoxelResolution", gi.voxelize_resolution);
    voxelize_shader->SetUniform("uWorldSize", gi.world_size);
    voxelize_shader->SetUniform("uVoxelCenter", gi.voxelize_center);
    for (auto [entity, mesh_data] : view.each())
    {
        if (mesh_data.mesh == nullptr)
            continue;
        glm::mat4 model = scene->GetSceneNodeByEntity(entity)->model;
        voxelize_shader->SetUniform("model", model);
        auto material = std::dynamic_pointer_cast<PBRMaterial>(mesh_data.mesh->GetMaterial());
        Texture::ResetSlot();
        auto uAlbedoTexEnabled = utils::SetMaterialUniform(material->albedo, voxelize_shader, "albedo", utils::MatUniValue::Vec3);
        voxelize_shader->SetUniform("uAlbedoTexEnabled", static_cast<float>(uAlbedoTexEnabled));
        mesh_data.mesh->Draw();
    }
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
void VoxelGlobalIllumination::DrawVoxelScene(Scene::Ptr &scene, glm::ivec2 viewport_size, component::VoxelGlobalIllumination &gi)
{
    if (vxgi_visualize_rt == nullptr)
    {
        voxel_vis_shader = ShaderManager::Instance().LoadShaderAndGet(
            "Resource/Shader/VXGI/VoxelVisualization.vert", "Resource/Shader/VXGI/VoxelVisualization.frag");
        voxel_face_shader = ShaderManager::Instance().LoadShaderAndGet(
            "Resource/Shader/VXGI/VoxelizeWorldCube.vert", "Resource/Shader/VXGI/VoxelizeWorldCube.frag");
        vxgi_visualize_rt = std::make_shared<WriteToTexture>(viewport_size.x, viewport_size.y);

        vxgi_visualize_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, viewport_size.x, viewport_size.y,
                                                   Texture::Format::RGBA, Texture::Format::RGBA16F, Texture::DataType::FLOAT);
        vxgi_visualize_rt->GetTexture(0)->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        vxgi_visualize_rt->GetTexture(0)->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        vxgi_visualize_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, viewport_size.x, viewport_size.y, GL_DEPTH_COMPONENT16);
        vxgi_visualize_rt->Unbind();
        std::cout << "VoxelGlobalIllumination Debug RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
    }

    vxgi_visualize_rt->Rescale(viewport_size.x, viewport_size.y);
    vxgi_visualize_rt->Bind();
    glViewport(0,0,viewport_size.x, viewport_size.y);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto registry = scene->GetRegistry();
    auto &camera = scene->GetCurrentCameraComponent();
    auto view = registry->view<component::MeshData>();
    voxel_vis_shader->SetUniform("view", camera.GetViewMatrix());
    voxel_vis_shader->SetUniform("projection", camera.GetProjectionMatrix());
    voxel_vis_shader->SetUniform("uWorldSize", gi.world_size);
    voxel_vis_shader->SetUniform("uVoxelResolution", gi.voxelize_resolution);
    voxel_vis_shader->SetUniform("uVoxelCenter", gi.voxelize_center);
    voxel_albedo->BindImage(0, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
    for (auto [entity, mesh_data] : view.each())
    {
        if (mesh_data.mesh == nullptr)
            continue;
        glm::mat4 model = scene->GetSceneNodeByEntity(entity)->model;
        voxel_vis_shader->SetUniform("model", model);
        mesh_data.mesh->Draw();
    }

    vxgi_visualize_rt->Unbind();
}
NoobRenderer::WriteToTexture::Ptr &VoxelGlobalIllumination::GetDrawVoxelScene()
{
    return vxgi_visualize_rt;
}
void VoxelGlobalIllumination::ClearALLDebugRT()
{
    vxgi_visualize_rt.reset();
    voxel_vis_shader.reset();
}
NoobRenderer::WriteToTexture::Ptr &VoxelGlobalIllumination::GetVoxelizeRT()
{
    return vxgi_rt;
}