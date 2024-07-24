#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#include "Graphics/Backend/OpenGL/GLPipeline.h"
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

void GLCommandBuffer::BindPipeline(const SharedPtr<Pipeline> &pipeline)
{
    if (pipeline != mBoundPipeline || mBoundPipelineLayer != 0)
    {
        mBoundPipelineLayer = 0;
        pipeline->Bind(this->Get());
        this->EmulateRecording(GLCommandCall([&]()
                                             { mBoundPipeline = pipeline; }));
        // mBoundPipeline = pipeline;
    }
}

void GLCommandBuffer::BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer)
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

void GLCommandBuffer::EmulateRecording(GLCommandCall &&_call)
{
    if (!mRecording)
        return;
    mCmds.emplace_back(std::forward<GLCommandCall>(_call));
}