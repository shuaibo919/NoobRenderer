#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
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
        using ShaderJson = nlohmann::json;
        class Shader : public Asset, public RHIBase
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
                std::map<uint32_t, DescriptorSetInfo> reflectInfo;
            };
            struct Builder final : BuildBase<Shader::Properties, RenderPass>
            {
                Builder() {}
                Shader::Builder &SetFile(const std::string &filePath);
                Shader::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual void Bind() const {};
            virtual void Unbind() const {};

            virtual ~Shader();
            virtual bool IsCompiled() = 0;

        public:
            const Properties &GetProperties() const { return *mProperties; }

        protected:
            void ReadReflectInfo(ShaderJson &info, ShaderType type);

        protected:
            Properties *mProperties;
            Shader(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
