#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/Serialization.h"
namespace NoobRenderer
{
    namespace component
    {
        // beta
        // https://jose-villegas.github.io/post/deferred_voxel_shading/
        struct VoxelGlobalIllumination
        {
            int voxel_size;
            SERIALIZATION_COMPONENT_ENABLE(VoxelGlobalIllumination, voxel_size)
            SERIALIZATION_FROM_TYPE_ANY(VoxelGlobalIllumination)
        };

    } // namespace component

};
