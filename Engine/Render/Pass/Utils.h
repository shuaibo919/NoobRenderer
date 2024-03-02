#pragma once
#include "Engine/Function/Graphics/Texture/All.h"
#include "Engine/Function/Graphics/Material/All.h"
#include "Engine/Function/Graphics/Shader.h"

namespace NoobRenderer
{
    namespace render
    {
        namespace passutils
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
