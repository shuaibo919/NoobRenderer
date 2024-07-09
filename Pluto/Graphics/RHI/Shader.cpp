#include "Graphics/RHI/Shader.h"
#include "Graphics/RHI/GraphicsContext.h"

using namespace pluto::Graphics;

void to_json(ShaderJson &j, const ShaderType &p)
{
    switch (p)
    {
    case ShaderType::Vertex:
        j = "Vertex";
        break;
    case ShaderType::Fragment:
        j = "Fragment";
        break;
    case ShaderType::Geometry:
        j = "Geometry";
        break;
    case ShaderType::Compute:
        j = "Compute";
    default:
        break;
    }
    j = "Unkown";
}
void from_json(const ShaderJson &j, ShaderType &p)
{
    if (j == "Vertex")
        p = ShaderType::Vertex;
    else if (j == "Fragment")
        p = ShaderType::Fragment;
    else if (j == "Geometry")
        p = ShaderType::Geometry;
    else if (j == "Compute")
        p = ShaderType::Compute;
    p = ShaderType::Unkown;
}

Shader::Builder &Shader::Builder::SetFile(const std::string &filePath)
{
    mProperties->filePath = filePath;
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