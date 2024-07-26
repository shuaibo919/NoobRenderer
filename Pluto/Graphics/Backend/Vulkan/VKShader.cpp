/* Vulkan Shader */
#include "Graphics/Backend/Vulkan/VKShader.h"
#include "Graphics/RHI/Shader.h"
/* Usage */
#include "Core/FileSystem.h"
#include "Core/Utilities.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;

VKShader::VKShader(RenderContext *ctx, VKShader::Properties *&&pProperties)
    : Shader(ctx, std::move(pProperties))
{
}

VKShader::~VKShader()
{
}

void VKShader::Bind() const
{
}

void VKShader::Unbind() const
{
}

bool VKShader::IsCompiled()
{
    return false;
}