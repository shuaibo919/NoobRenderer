#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#include "Graphics/Backend/OpenGL/GLPipeline.h"
using namespace pluto::Graphics;

GLCommandBuffer::GLCommandBuffer(RenderContext *ctx, Properties *&&pProperties)
    : primary(false), CommandBuffer(ctx, std::move(pProperties))
{
}

GLCommandBuffer::~GLCommandBuffer()
{
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
}

void GLCommandBuffer::BeginRecordingSecondary(std::shared_ptr<RenderPass> &renderPass, std::shared_ptr<Framebuffer> &framebuffer)
{
}

void GLCommandBuffer::EndRecording()
{
}

void GLCommandBuffer::ExecuteSecondary(std::shared_ptr<CommandBuffer> &primaryCmdBuffer)
{
}

void GLCommandBuffer::BindPipeline(std::shared_ptr<Pipeline> &pipeline)
{
    if (pipeline != mBoundPipeline || mBoundPipelineLayer != 0)
    {
        mBoundPipelineLayer = 0;
        pipeline->Bind(this->Get());
        mBoundPipeline = pipeline;
    }
}

void GLCommandBuffer::BindPipeline(std::shared_ptr<Pipeline> &pipeline, uint32_t layer)
{
    if (pipeline != mBoundPipeline || mBoundPipelineLayer != layer)
    {
        pipeline->Bind(this->Get(), layer);
        mBoundPipeline = pipeline;
        mBoundPipelineLayer = layer;
    }
}

void GLCommandBuffer::UnBindPipeline()
{
    if (mBoundPipeline)
        mBoundPipeline->End(this->Get());
    mBoundPipeline = nullptr;
}

GLCommandBuffer::Ptr GLCommandBuffer::Get()
{
    return shared_from_this();
}