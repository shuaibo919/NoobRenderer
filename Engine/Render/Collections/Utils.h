#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"
#include "Engine/Core/Utility.h"
#include "Engine/Function/Component/All.h"
#include "Engine/Function/Buffer/All.h"
#include "Engine/Function/Graphics/Texture/All.h"
#include "Engine/Function/Scene.h"
#include "Engine/Function/Graphics/Shader.h"
namespace NoobRenderer
{
    namespace render
    {
        namespace utils
        {
            enum class MatUniValue
            {
                None,
                Float,
                Vec3
            };
            bool SetMaterialUniform(MaterialTexture::Ptr &mtex, ShaderProgram::Ptr &shader, const std::string &name, MatUniValue op);
            bool SetMaterialUniform(MaterialTexture::Ptr &mtex, ShaderProgram::Ptr &shader);
        }
    }
}