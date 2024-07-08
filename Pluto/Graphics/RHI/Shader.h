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
        class Pipeline;
        class CommandBuffer;

        class Shader : public Asset
        {
        public:
            static const Shader *sCurrently;

        public:
            using Ptr = std::shared_ptr<Shader>;
            struct Properties
            {
                std::string name;
                std::string filePath;
                std::vector<ShaderType> types;
                std::vector<PushConstant> pushConstants;
                DescriptorSetInfo descriptorSetInfo;

                uint32_t *vertData;
                uint32_t vertDataSize;
                uint32_t *fragData;
                uint32_t fragDataSize;
                uint32_t *geomData;
                uint32_t geomDataSize;
                uint32_t *compData;
                uint32_t compDataSize;
            };
            struct Builder final : BuildBase<Shader::Properties, RenderPass>
            {
                Builder() {}
                Shader::Builder &SetFile(const std::string &filePath);
                Shader::Builder &SetVertData(uint32_t *vertData, uint32_t size);
                Shader::Builder &SetFragData(uint32_t *fragData, uint32_t size);
                Shader::Builder &SetGeomData(uint32_t *geomData, uint32_t size);
                Shader::Builder &SetCompData(uint32_t *compData, uint32_t size);
                Shader::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual void Bind() const {};
            virtual void Unbind() const {};

            virtual ~Shader();

            virtual bool IsCompiled() = 0;
            virtual void BindPushConstants(std::shared_ptr<CommandBuffer> commandBuffer, std::shared_ptr<Pipeline> pipeline) = 0;
            ShaderDataType ConvertSPIRV(const spirv_cross::SPIRType type);

        protected:
            Properties *mProperties;
            Shader(Properties *&&pProperties);
        };
    }
}
