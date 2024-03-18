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
            int voxelize_resolution{256};
            /*
             * @breif used to build the voxelizing view and projection matrices
             */
            glm::vec3 voxelize_center{0.0f, 0.0f, 0.0f};
            float world_size{20.0f};
            SERIALIZATION_COMPONENT_ENABLE(VoxelGlobalIllumination, voxelize_resolution, voxelize_center, world_size)
            SERIALIZATION_FROM_TYPE_ANY(VoxelGlobalIllumination)
        };

    } // namespace component

};
