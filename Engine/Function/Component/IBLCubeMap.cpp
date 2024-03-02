#include "Engine/Function/Component/IBLCubeMap.h"
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"
#include "Engine/Core/Utility.h"
#include "Engine/Function/Buffer/All.h"
#include "Engine/Function/Graphics/ShaderManager.h"
#include "Engine/Function/Graphics/MeshManager.h"
#include "Engine/Function/Utility/Helpers.h"
namespace NoobRenderer
{
    namespace component
    {
        std::vector<glm::mat4> GetCaptureViews()
        {
            std::vector<glm::mat4> tmp(6);
            tmp[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
            tmp[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
            tmp[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            tmp[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
            tmp[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
            tmp[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
            return tmp;
        }
        CubeMapTexture::Ptr CreateFromHDRTexture(unsigned int width, unsigned int height, HDRBufferTexture::Ptr &hdr_texture)
        {
            auto fbo = std::make_shared<FrameBuffer>();
            auto rbo = std::make_shared<RenderBuffer>(width, height, GL_DEPTH_COMPONENT24);
            fbo->SetRenderBuffer(GL_DEPTH_ATTACHMENT, rbo->GetID());
            TextureParameter tex_param;
            tex_param.Add(gtype::TexParaType::WrapR, gtype::TexPara::ClampEdge);
            tex_param.Add(gtype::TexParaType::WrapS, gtype::TexPara::ClampEdge);
            tex_param.Add(gtype::TexParaType::WrapT, gtype::TexPara::ClampEdge);
            tex_param.Add(gtype::TexParaType::MinFilter, gtype::TexPara::LinearMipmapLinear);
            tex_param.Add(gtype::TexParaType::MagFilter, gtype::TexPara::Linear);
            auto tex = std::make_shared<CubeMapTexture>(width, height, gtype::Format::RGB, gtype::Format::RGB16F, gtype::DataType::FLOAT, tex_param);
            auto shader = ShaderManager::Instance().LoadShaderAndGet("Resource/Shader/HDRToCubeMap.vert", "Resource/Shader/HDRToCubeMap.frag");
            glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
            auto captureViews = GetCaptureViews();
            Texture::ResetSlot();
            hdr_texture->Activate();
            shader->SetUniform("equirectangularMap", hdr_texture->GetTempSlot());
            shader->SetUniform("projection", captureProjection);
            glViewport(0, 0, width, height);
            fbo->Bind();
            rbo->Bind();
            auto meshdata = MeshManager::Instance().GetMesh("Cube");
            for (auto i = 0; i < captureViews.size(); ++i)
            {
                shader->SetUniform("view", captureViews[i]);
                fbo->SetTexture2D(GL_COLOR_ATTACHMENT0, tex->GetID(), GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                meshdata->Draw();
            }
            tex->GenerateMipmap();
            rbo->Unbind();
            std::cout << "IBLComponent::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            fbo->Unbind();
            return tex;
        }
        CubeMapTexture::Ptr CreateConvolutionEnvMap(unsigned int width, unsigned int height, CubeMapTexture::Ptr &cubemap)
        {
            auto fbo = std::make_shared<FrameBuffer>();
            auto rbo = std::make_shared<RenderBuffer>(width, height, GL_DEPTH_COMPONENT24);
            fbo->SetRenderBuffer(GL_DEPTH_ATTACHMENT, rbo->GetID());
            TextureParameter tex_param;
            tex_param.Add(gtype::TexParaType::WrapR, gtype::TexPara::ClampEdge);
            tex_param.Add(gtype::TexParaType::WrapS, gtype::TexPara::ClampEdge);
            tex_param.Add(gtype::TexParaType::WrapT, gtype::TexPara::ClampEdge);
            tex_param.Add(gtype::TexParaType::MinFilter, gtype::TexPara::Linear);
            tex_param.Add(gtype::TexParaType::MagFilter, gtype::TexPara::Linear);
            auto tex = std::make_shared<CubeMapTexture>(width, height, gtype::Format::RGB, gtype::Format::RGB16F,
                                                        gtype::DataType::FLOAT, tex_param);

            auto shader = ShaderManager::Instance().LoadShaderAndGet("Resource/Shader/ConvCubeMap.vert", "Resource/Shader/ConvCubeMap.frag");
            auto captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
            auto captureViews = GetCaptureViews();
            Texture::ResetSlot();
            cubemap->Activate();
            shader->SetUniform("environmentMap", cubemap->GetTempSlot());
            shader->SetUniform("projection", captureProjection);

            glViewport(0, 0, width, height); // don't forget to configure the viewport to the capture dimensions.
            fbo->Bind();
            rbo->Bind();
            auto meshdata = MeshManager::Instance().GetMesh("Cube");
            for (unsigned int i = 0; i < captureViews.size(); ++i)
            {
                shader->SetUniform("view", captureViews[i]);
                fbo->SetTexture2D(GL_COLOR_ATTACHMENT0, tex->GetID(), GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                meshdata->Draw();
            }
            rbo->Unbind();
            std::cout << "IBLComponent::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            fbo->Unbind();
            return tex;
        }
        CubeMapTexture::Ptr CreatePreFilterCubeMap(unsigned int width, unsigned int height, CubeMapTexture::Ptr &cubemap)
        {
            auto fbo = std::make_shared<FrameBuffer>();
            auto rbo = std::make_shared<RenderBuffer>(width, height, GL_DEPTH_COMPONENT24);
            fbo->SetRenderBuffer(GL_DEPTH_ATTACHMENT, rbo->GetID());
            TextureParameter tex_param;
            tex_param.Add(gtype::TexParaType::WrapR, gtype::TexPara::ClampEdge);
            tex_param.Add(gtype::TexParaType::WrapS, gtype::TexPara::ClampEdge);
            tex_param.Add(gtype::TexParaType::WrapT, gtype::TexPara::ClampEdge);
            tex_param.Add(gtype::TexParaType::MinFilter, gtype::TexPara::LinearMipmapLinear);
            tex_param.Add(gtype::TexParaType::MagFilter, gtype::TexPara::Linear);
            auto tex = std::make_shared<CubeMapTexture>(width, height, gtype::Format::RGB, gtype::Format::RGB16F,
                                                        gtype::DataType::FLOAT, tex_param);
            tex->GenerateMipmap();
            auto shader = ShaderManager::Instance().LoadShaderAndGet("Resource/Shader/Prefilter.vert", "Resource/Shader/Prefilter.frag");
            auto captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
            auto meshdata = MeshManager::Instance().GetMesh("Cube");
            auto captureViews = GetCaptureViews();
            Texture::ResetSlot();
            cubemap->Activate();
            shader->SetUniform("environmentMap", cubemap->GetTempSlot());
            shader->SetUniform("projection", captureProjection);

            fbo->Bind();
            rbo->Bind();
            unsigned int maxMipLevels = 5;
            for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
            {
                // reisze framebuffer according to mip-level size.
                unsigned int mipWidth = static_cast<unsigned int>(width * std::pow(0.5, mip));
                unsigned int mipHeight = static_cast<unsigned int>(height * std::pow(0.5, mip));
                rbo->Bind();
                rbo->Rescale(mipWidth, mipHeight);
                glViewport(0, 0, mipWidth, mipHeight);
                float roughness = (float)mip / (float)(maxMipLevels - 1);
                shader->SetUniform("roughness", roughness);
                for (unsigned int i = 0; i < captureViews.size(); ++i)
                {
                    shader->SetUniform("view", captureViews[i]);
                    fbo->SetTexture2D(GL_COLOR_ATTACHMENT0, tex->GetID(), GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    meshdata->Draw();
                }
            }
            rbo->Unbind();
            std::cout << "IBLComponent::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            fbo->Unbind();
            return tex;
        }
        LookUpTexture::Ptr CreateBRDFLookUpTexture(unsigned int width, unsigned int height)
        {
            using namespace NoobRenderer;
            auto fbo = std::make_shared<FrameBuffer>();
            auto rbo = std::make_shared<RenderBuffer>(width, height, GL_DEPTH_COMPONENT24);
            fbo->SetRenderBuffer(GL_DEPTH_ATTACHMENT, rbo->GetID());
            auto tex = std::make_shared<LookUpTexture>(width, height, gtype::Format::RG, gtype::Format::RG16F, gtype::DataType::FLOAT);
            fbo->SetTexture2D(GL_COLOR_ATTACHMENT0, tex->GetID(), GL_TEXTURE_2D);
            auto shader = ShaderManager::Instance().LoadShaderAndGet("Resource/Shader/CalcBRDF.vert", "Resource/Shader/CalcBRDF.frag");
            fbo->Bind();
            rbo->Bind();
            tex->Bind();
            {
                glViewport(0, 0, width, height);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                shader->Use();
                EngineUtility::DrawQuad();
            }
            rbo->Unbind();
            std::cout << "IBLComponent::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            fbo->Unbind();
            return tex;
        }
        void IBLCubeMap::ConductPreCalculation(void *&typeAny)
        {
            auto ptr = static_cast<IBLCubeMap *>(typeAny);
            if (ptr->name.empty())
                return;
            ptr->RawHDRTexture = std::make_shared<HDRBufferTexture>(ptr->name);
            ptr->HDRCubeMap = CreateFromHDRTexture(ptr->cubemap_size, ptr->cubemap_size, ptr->RawHDRTexture);
            ptr->IrridianceMap = CreateConvolutionEnvMap(ptr->conv_size, ptr->conv_size, ptr->HDRCubeMap);
            ptr->PrefilterMap = CreatePreFilterCubeMap(ptr->mipmap_size, ptr->mipmap_size, ptr->HDRCubeMap);
            ptr->BRDFLookupTexture = CreateBRDFLookUpTexture(ptr->brdf_size, ptr->brdf_size);
            ptr->valid = true;
        }
        void IBLCubeMap::PreCalculation()
        {
            if (this->name.empty())
                return;
            this->RawHDRTexture = std::make_shared<HDRBufferTexture>(this->name);
            this->HDRCubeMap = CreateFromHDRTexture(this->cubemap_size, this->cubemap_size, this->RawHDRTexture);
            this->IrridianceMap = CreateConvolutionEnvMap(this->conv_size, this->conv_size, this->HDRCubeMap);
            this->PrefilterMap = CreatePreFilterCubeMap(this->mipmap_size, this->mipmap_size, this->HDRCubeMap);
            this->BRDFLookupTexture = CreateBRDFLookUpTexture(this->brdf_size, this->brdf_size);
            this->valid = true;
        }
    }
}