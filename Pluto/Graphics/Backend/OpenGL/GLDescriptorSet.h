#pragma once

#include "Graphics/RHI/DescriptorSet.h"

namespace pluto
{
    namespace Graphics
    {
        class GLShader;
        class GLDescriptorSet : public DescriptorSet
        {
            friend class GLContext;

        public:
            GLDescriptorSet(GLDescriptorSet::Properties *&&pProperties);
            ~GLDescriptorSet();
        };
    }
}
