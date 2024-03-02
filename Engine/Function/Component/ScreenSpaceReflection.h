#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/Serialization.h"
namespace NoobRenderer
{
    namespace component
    {
        struct ScreenSpaceReflection
        {
            static const unsigned int stentil_value = 1 << 0;
            bool enable{true};
            SERIALIZATION_COMPONENT_ENABLE(ScreenSpaceReflection, enable)
            SERIALIZATION_FROM_TYPE_ANY(ScreenSpaceReflection)
        };

    } // namespace component

};
