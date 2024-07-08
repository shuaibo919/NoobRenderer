#include "Graphics/RHI/Shader.h"
#include "Graphics/RHI/GraphicsContext.h"
using namespace pluto::Graphics;

Shader::Builder &Shader::Builder::SetFile(const std::string &filePath)
{
    mProperties->filePath = filePath;
    return *this;
}

Shader::Builder &Shader::Builder::SetVertData(uint32_t *vertData, uint32_t size)
{
    mProperties->vertData = vertData;
    mProperties->vertDataSize = size;
    return *this;
}

Shader::Builder &Shader::Builder::SetFragData(uint32_t *fragData, uint32_t size)
{
    mProperties->fragData = fragData;
    mProperties->fragDataSize = size;
    return *this;
}

Shader::Builder &Shader::Builder::SetGeomData(uint32_t *geomData, uint32_t size)
{
    mProperties->geomData = geomData;
    mProperties->geomDataSize = size;
    return *this;
}

Shader::Builder &Shader::Builder::SetCompData(uint32_t *compData, uint32_t size)
{
    mProperties->compData = compData;
    mProperties->compDataSize = size;
    return *this;
}

Shader::Ptr Shader::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreateShader(std::move(mProperties));
}

Shader::Shader(Properties *&&pProperties)
    : mProperties(pProperties)
{
}

Shader::~Shader()
{
    if (mProperties != nullptr)
        delete mProperties;
}