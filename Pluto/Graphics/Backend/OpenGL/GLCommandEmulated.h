#pragma once
#include "Graphics/RHI/Declarations.h"
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#define GlNativeCmdCtor(x) GLCommandCall([&]() { GlCall(x); })

#define GlWrapCmdCtor(x) GLCommandCall([&]() { x; })

namespace pluto
{
    namespace Graphics
    {
        enum class GLEmulatedCommandType
        {
            BindPipeline,
            BindDescriptorSet,
            BindVertexBuffer,
            Draw,
        };

        struct GLEmulatedCommandData
        {
        };

        class GLEmulatedCommand
        {
        public:
            GLEmulatedCommand(GLEmulatedCommandData *&&pParams, GLEmulatedCommandType type);
            GLEmulatedCommand(GLEmulatedCommand &&other) noexcept;
            ~GLEmulatedCommand();
            void Execute();

        private:
            GLEmulatedCommandType mType;
            GLEmulatedCommandData *mParams;
        };

        struct CmdBindPipelineData : public GLEmulatedCommandData
        {
            Pipeline *pointer;
            uint32_t layer;
        };

        struct CmdBindDescriptorSetData : public GLEmulatedCommandData
        {
            DescriptorSet *pointer;
            uint32_t dynamicOffset;
        };

        struct CmdBindVertexBufferData : public GLEmulatedCommandData
        {
            VertexBuffer *pointer;
            SharedPtr<Pipeline> pipeline;
            uint32_t binding;
        };

        struct CmdDrawData : public GLEmulatedCommandData
        {
            unsigned int type;
            int first;
            int count;
        };

    }
}