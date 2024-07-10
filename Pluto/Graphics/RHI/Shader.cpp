#include "Graphics/RHI/Shader.h"
#include "Graphics/RHI/GraphicsContext.h"

using namespace pluto::Graphics;

Shader::Builder &Shader::Builder::SetFile(const std::string &filePath)
{
    mProperties->filePath = filePath;
    return *this;
}

Shader::Ptr Shader::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreateShader(std::move(mProperties));
}

Shader::Shader(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}

Shader::~Shader()
{
    if (mProperties != nullptr)
        delete mProperties;
}