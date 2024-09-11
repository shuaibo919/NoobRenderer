/* OpenGL Command Buffer */
#include "Graphics/Backend/OpenGL/GLCommandEmulated.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/Backend/OpenGL/GLVertexBuffer.h"
#include "Graphics/Backend/OpenGL/GLSwapChain.h"
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
#include "Graphics/Backend/OpenGL/GLPipeline.h"
#include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"
using namespace pluto;
using namespace pluto::Graphics;

GLEmulatedCommand::GLEmulatedCommand(GLEmulatedCommandData *&&pParams, GLEmulatedCommandType type)
    : mParams(pParams), mType(type)
{
}

GLEmulatedCommand::GLEmulatedCommand(GLEmulatedCommand &&other) noexcept
    : mParams(std::move(other.mParams)), mType(other.mType)
{
    other.mParams = nullptr;
}

GLEmulatedCommand::~GLEmulatedCommand()
{
    if (mParams != nullptr)
        delete mParams;
}

void GLEmulatedCommand::Execute()
{
    switch (mType)
    {
    case GLEmulatedCommandType::BindPipeline:
    {
        auto recipient = static_cast<CmdBindPipelineData *>(mParams);
        recipient->pointer->Bind(nullptr, recipient->layer);
        break;
    }
    case GLEmulatedCommandType::BindVertexBuffer:
    {
        auto recipient = static_cast<CmdBindVertexBufferData *>(mParams);
        recipient->pointer->Bind(nullptr, recipient->pipeline, recipient->binding);
        break;
    }
    case GLEmulatedCommandType::BindDescriptorSet:
    {
        auto recipient = static_cast<CmdBindDescriptorSetData *>(mParams);
        static_cast<GLDescriptorSet *>(recipient->pointer)->Bind(recipient->dynamicOffset);
        break;
    }
    case GLEmulatedCommandType::Draw:
    {
        auto recipient = static_cast<CmdDrawData *>(mParams);
        GlCall(glDrawArrays(recipient->type, recipient->first, recipient->count));
        break;
    }
    default:
        break;
    }
}