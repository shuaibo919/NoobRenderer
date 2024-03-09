#include "Engine/Render/Pass/ForwardPass.h"
#include "Engine/Render/Collections/Utils.h"
#include <random>
namespace NoobRenderer
{
    namespace render
    {
        void ForwardPass::Init(unsigned int width, unsigned int height)
        {
            m_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Forward/PBR.vert",
                "Resource/Shader/Forward/PBR.frag");
            m_screen_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Quad.vert",
                "Resource/Shader/Quad.frag");
            m_cubemap_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/CubeMap.vert",
                "Resource/Shader/CubeMap.frag");
            m_rt = std::make_shared<WriteToTexture>(width, height);
            m_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_STENCIL_ATTACHMENT, width, height, GL_DEPTH24_STENCIL8);
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height,
                                          Texture::Format::RGBA, Texture::Format::RGB16F, Texture::DataType::FLOAT);
            m_rt->GetTexture()->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_rt->GetTexture()->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            std::cout << "ForwardPass Screen::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            m_rt->Unbind();
            int samples = 4;
            m_msaa_offline = std::make_shared<WriteToTexture>(width, height);
            m_msaa_offline->SetRenderBuffer<MultiSampleRenderBuffer>(GL_DEPTH_STENCIL_ATTACHMENT, width, height, GL_DEPTH24_STENCIL8, samples);
            m_msaa_offline->SetTexture2D<MultiSampleTexture>(GL_COLOR_ATTACHMENT0, width, height, Texture::Format::RGB, samples);
            std::cout << "ForwardPass MSAA offline::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            m_msaa_offline->Unbind();
        }
        void ForwardPass::Rescale(unsigned int width, unsigned int height)
        {
            m_rt->Rescale(width, height);
            m_msaa_offline->Rescale(width, height);
        }
        void ForwardPass::Render(Scene::Ptr &scene, RenderStorage &input)
        {
            // m_msaa_offline->Bind();
            // SetLightUniforms(scene, input);
            // {
            //     glEnable(GL_DEPTH_TEST);
            //     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //     glViewport(0, 0, m_rt->GetWidth(), m_rt->GetHeight());
            //     SetSceneUniforms(scene, input);
            // }
            // m_msaa_offline->BlitFrom(m_rt);
            // m_msaa_offline->Unbind();
            // input.Insert(RenderStorageOutputKey[0], m_rt->GetTexture(0));
        }
        void ForwardPass::SetLightUniforms(Scene::Ptr &scene, RenderStorage &input)
        {
            //     auto registry = scene->GetRegistry();
            //     auto view = registry->view<component::PointLight>();
            //     auto light_count = 0;
            //     auto main_dir_light_enable = 0;
            //     for (auto [entity, pointlight] : view.each())
            //     {
            //         auto number = std::to_string(light_count++);
            //         m_shader->SetUniform("pointLights[" + number + "].position", pointlight.position);
            //         m_shader->SetUniform("pointLights[" + number + "].color", pointlight.color);
            //         m_shader->SetUniform("pointLights[" + number + "].constant", pointlight.constant);
            //         m_shader->SetUniform("pointLights[" + number + "].linear", pointlight.linear);
            //         m_shader->SetUniform("pointLights[" + number + "].quadratic", pointlight.quadratic);
            //         m_shader->SetUniform("pointLights[" + number + "].far", pointlight.far);
            //         m_shader->SetUniform("pointLights[" + number + "].castShadow", pointlight.shadow == true ? pointlight.shadow_channel : -1);
            //     }
            //     m_shader->SetUniform("nPointLights", light_count);
            //     auto dir_view = registry->view<component::DirectionalLight>();
            //     for (auto [entity, dirlight] : dir_view.each())
            //     {
            //         main_dir_light_enable++;
            //         m_shader->SetUniform("mainDirLight.direction", dirlight.direction);
            //         m_shader->SetUniform("mainDirLight.color", dirlight.color);
            //         m_shader->SetUniform("mainDirLight.intensity", dirlight.intensity);
            //         m_shader->SetUniform("mainDirLight.castShadow", static_cast<float>(dirlight.shadow));
            //         m_shader->SetUniform("mainDirLight.cascadeCount", static_cast<int>(dirlight.cascade_levels.size()));
            //         m_shader->SetUniform("mainDirLight.biasFactor", dirlight.biasFactor);
            //         m_shader->SetUniform("mainDirLight.minBias", dirlight.minBias);
            //         for (int i = 0; i < dirlight.cascade_levels.size(); i++)
            //         {
            //             auto number = std::to_string(i);
            //             m_shader->SetUniform("cascadePlaneDistances[" + number + "]", dirlight.cascade_levels[i]);
            //             m_shader->SetUniform("cascadeBiasModifier[" + number + "]", dirlight.biasModifier[i]);
            //         }
            //         break;
            //     }
            //     m_shader->SetUniform("mainDirLight.enable", static_cast<float>(main_dir_light_enable));
            // }
            // void ForwardPass::SetSceneUniforms(Scene::Ptr &scene, RenderStorage &input)
            // {
            //     auto registry = scene->GetRegistry();
            //     auto view = registry->view<component::MeshData>();
            //     auto &root = scene->GetRootNode();
            //     auto &camera = scene->GetCurrentCameraComponent();
            //     m_shader->SetUniform("viewPos", camera.GetPositionVector());
            //     m_shader->SetUniform("view", camera.GetViewMatrix());
            //     m_shader->SetUniform("projection", camera.GetProjectionMatrix());
            //     Texture::ResetSlot();
            //     {
            //         for (auto &RTName : RenderStorageInputKey)
            //         {
            //             input.Get(RTName)->Activate();
            //             m_shader->SetUniform(RTName, input.Get(RTName)->GetTempSlot());
            //         }
            //     }
            //     auto cubemap = root->object->TryGetComponent<component::IBLCubeMap>();
            //     if (cubemap != nullptr && cubemap->valid)
            //     {
            //         auto &irridance = cubemap->IrridianceMap;
            //         auto &prefilter = cubemap->PrefilterMap;
            //         auto &brdflut = cubemap->BRDFLookupTexture;
            //         auto has_IrradianceMap = SetTextureUniform<CubeMapTexture>(irridance, m_shader, "irradianceMap");
            //         auto has_PrefilterMap = SetTextureUniform<CubeMapTexture>(prefilter, m_shader, "prefilterMap");
            //         auto has_brdflut = SetTextureUniform<LookUpTexture>(brdflut, m_shader, "brdfLUT");
            //         m_shader->SetUniform("hasIrradianceMap", has_IrradianceMap);
            //         m_shader->SetUniform("hasPrefilterMap", has_PrefilterMap);
            //         m_shader->SetUniform("hasBrdfLUT", has_brdflut);
            //     }
            //     auto slot_start_pos = Texture::GetSlot() == 0 ? 0 : Texture::GetSlot() + 1;
            //     for (auto [entity, meshdata] : view.each())
            //     {
            //         m_shader->SetUniform("model", scene->GetSceneNodeByEntity(entity)->model);
            //         auto &mesh_ptr = meshdata.mesh;
            //         if (mesh_ptr == nullptr)
            //             continue;
            //         auto material = std::dynamic_pointer_cast<PBRMaterial>(mesh_ptr->GetMaterial());
            //         Texture::SpecifySlot(slot_start_pos);
            //         auto has_albedo = passutils::SetMaterialUniform(material->albedo, m_shader, "albedo", passutils::MatUniValue::Vec3);
            //         auto has_emission = passutils::SetMaterialUniform(material->emission, m_shader, "emission", passutils::MatUniValue::Vec3);
            //         // auto has_ao = passutils::SetMaterialUniform(material->ao, m_shader, "ao", passutils::MatUniValue::Float);
            //         auto has_metallic = passutils::SetMaterialUniform(material->metallic, m_shader, "metallic", passutils::MatUniValue::Float);
            //         auto has_roughness = passutils::SetMaterialUniform(material->roughness, m_shader, "roughness", passutils::MatUniValue::Float);
            //         auto has_normal = passutils::SetMaterialUniform(material->normal, m_shader);
            //         // auto has_height = SetMaterialUniform(material->height, m_lightShader);
            //         m_shader->SetUniform("hasAlbedo", static_cast<float>(has_albedo));
            //         m_shader->SetUniform("hasEmission", static_cast<float>(has_emission));
            //         m_shader->SetUniform("hasMetallic", static_cast<float>(has_metallic));
            //         // m_shader->SetUniform("hasAmbientOcclusion", static_cast<float>(has_ao));
            //         m_shader->SetUniform("hasRoughness", static_cast<float>(has_roughness));
            //         m_shader->SetUniform("hasNormalMap", static_cast<float>(has_normal));
            //         m_shader->SetUniform("hasHeightMap", false);
            //         mesh_ptr->Draw();
            //     }
            //     if (cubemap != nullptr && cubemap->valid)
            //     {
            //         glDepthFunc(GL_LEQUAL);
            //         glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
            //         auto cube = MeshManager::Instance().GetMesh("Cube");
            //         Texture::ResetSlot();
            //         cubemap->HDRCubeMap->Activate();
            //         m_cubemap_shader->SetUniform("environmentMap", cubemap->HDRCubeMap->GetTempSlot());
            //         m_cubemap_shader->SetUniform("projection", camera.GetProjectionMatrix());
            //         m_cubemap_shader->SetUniform("view", camera.GetViewMatrix());
            //         cube->Draw();
            //         glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
            //         glDepthFunc(GL_LESS);
            //     }
        }

    }
}