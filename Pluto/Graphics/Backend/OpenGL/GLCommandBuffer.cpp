/* OpenGL Command Buffer */
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#include "Graphics/Backend/OpenGL/GLCommandCall.h"
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

GLCommandBuffer::GLCommandBuffer(RenderContext *ctx, Properties *&&pProperties)
    : primary(false), mRecording(false), CommandBuffer(ctx, std::move(pProperties))
{
}

GLCommandBuffer::~GLCommandBuffer()
{
}

void GLCommandBuffer::Submit()
{
    int i = 0;
    for (auto &cmd : mCmds)
    {
        cmd.call();
        // log<Info>("Executing command: {%d}", i++);
    }
}

bool GLCommandBuffer::Init(bool primary)
{
    return true;
}

void GLCommandBuffer::Unload()
{
}

void GLCommandBuffer::BeginRecording()
{
    if (mRecording)
    {
        log<Error>("CommandBuffer is in recording state!");
        return;
    }
    mRecording = true;
    mCmds.clear();
}

void GLCommandBuffer::BeginRecordingSecondary(const SharedPtr<RenderPass> &renderPass, const SharedPtr<Framebuffer> &framebuffer)
{
}

void GLCommandBuffer::EndRecording()
{
    mRecording = false;
}

void GLCommandBuffer::ExecuteSecondary(const SharedPtr<CommandBuffer> &primaryCmdBuffer)
{
}

// void GLCommandBuffer::BindPipeline(const SharedPtr<Pipeline> &pipeline)
// {
//     if (pipeline != mBoundPipeline || mBoundPipelineLayer != 0)
//     {
//         mBoundPipelineLayer = 0;
//         pipeline->Bind(this->Get());
//         this->EmulateRecording(GLCommandCall([&]()
//                                              { mBoundPipeline = pipeline; }));
//         // mBoundPipeline = pipeline;
//     }
// }

// void GLCommandBuffer::BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer)
// {
//     if (pipeline != mBoundPipeline || mBoundPipelineLayer != layer)
//     {
//         pipeline->Bind(this->Get(), layer);
//         mBoundPipeline = pipeline;
//         mBoundPipelineLayer = layer;
//     }
// }

// void GLCommandBuffer::UnBindPipeline()
// {
//     if (mBoundPipeline)
//         mBoundPipeline->End(this->Get());
//     mBoundPipeline = nullptr;
// }

GLCommandBuffer::Ptr GLCommandBuffer::Get()
{
    return shared_from_this();
}

void GLCommandBuffer::EmulateRecording(GLCommandCall &&_call)
{
    if (!mRecording)
        return;
    mCmds.emplace_back(std::forward<GLCommandCall>(_call));
}

void GLCommandBuffer::Present(const SharedPtr<CommandBuffer> &commandBuffer)
{
}

void GLCommandBuffer::BindVetexBuffer(const SharedPtr<Pipeline> &pipeline, const SharedPtr<VertexBuffer> &vbo, uint8_t binding)
{
    auto BindVetexBuffer = [&]()
    {
        vbo->Bind(nullptr, pipeline, binding);
    };
    this->EmulateRecording(GLCommandCall(std::move(BindVetexBuffer)));
}

void GLCommandBuffer::BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet)
{
    if (descriptorSet != nullptr)
    {
        this->EmulateRecording(GlWrapCmdCtor(std::static_pointer_cast<GLDescriptorSet>(descriptorSet)->Bind(dynamicOffset)));
    }
}

void GLCommandBuffer::BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets)
{
}

void GLCommandBuffer::UpdateViewport(uint32_t width, uint32_t height, bool flipViewport)
{
}

void GLCommandBuffer::BindPipeline(const SharedPtr<Pipeline> &pipeline)
{
    auto BindPipeline = [&]()
    {
        if (pipeline != mBoundPipeline || mBoundPipelineLayer != 0)
        {
            mBoundPipelineLayer = 0;
            pipeline->Bind(nullptr);
            mBoundPipeline = pipeline;
        }
        if (pipeline == mBoundPipeline)
        {
            pipeline->ClearRenderTargets(nullptr);
        }
    };
    this->EmulateRecording(GLCommandCall(std::move(BindPipeline)));
}

void GLCommandBuffer::BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer)
{
    auto BindPipeline = [&]()
    {
        if (pipeline != mBoundPipeline || mBoundPipelineLayer != layer)
        {
            pipeline->Bind(this->Get(), layer);
            mBoundPipeline = pipeline;
            mBoundPipelineLayer = layer;
        }
    };
    this->EmulateRecording(GLCommandCall(std::move(BindPipeline)));
}

void GLCommandBuffer::DrawIndexed(DrawType type, uint32_t count, uint32_t start)
{
    this->EmulateRecording(GlNativeCmdCtor(glDrawElements(GLUtilities::GetDrawType(type), count, GLUtilities::GetDataType(DataType::UnsignedInt), nullptr)));
}

void GLCommandBuffer::Draw(DrawType type, uint32_t count)
{
    this->EmulateRecording(GlNativeCmdCtor(glDrawArrays(GLUtilities::GetDrawType(type), 0, count)));
}

void GLCommandBuffer::Dispatch(uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ)
{
}

void GLCommandBuffer::DrawSplashScreen(const SharedPtr<Texture> &texture)
{
}

void GLCommandBuffer::UnBindPipeline()
{
}

void GLCommandBuffer::EndCurrentRenderPass()
{
}