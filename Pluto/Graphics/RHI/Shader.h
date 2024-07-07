#pragma once
#include "Graphics/RHI/Declarations.h"

namespace spirv_cross
{
    struct SPIRType;
}
namespace pluto
{
    namespace Graphics
    {
        class Shader : public Asset
        {
        public:
            static const Shader *sCurrently;

        public:
            //     using Ptr = std::shared_ptr<Shader>;
            //     struct Properties
            //     {
            //     };
            //     struct Builder final : BuildBase<Shader::Properties, RenderPass>
            //     {
            //         Builder() {}
            //         Shader::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            //     };
            virtual void Bind() const{};
            virtual void Unbind() const{};

            //     virtual ~Shader() = default;

            //     virtual const std::vector<ShaderType> GetShaderTypes() const = 0;
            //     virtual const std::string &GetName() const = 0;
            //     virtual const std::string &GetFilePath() const = 0;

            //     virtual bool IsCompiled() const { return true; }

            //     virtual std::vector<PushConstant> &GetPushConstants() = 0;
            //     virtual PushConstant *GetPushConstant(uint32_t index) { return nullptr; }
            //     virtual void BindPushConstants(Graphics::CommandBuffer *commandBuffer, Graphics::Pipeline *pipeline) = 0;
            //     virtual DescriptorSetInfo GetDescriptorInfo(uint32_t index) { return DescriptorSetInfo(); }
            //     virtual uint64_t GetHash() const { return 0; };

            //     ShaderDataType ConvertSPIRV(const spirv_cross::SPIRType type);

            // protected:
            //     Properties *mProperties;
            //     Shader(Properties *&&pProperties);
        };
    }
}
