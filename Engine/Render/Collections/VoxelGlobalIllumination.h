#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"
#include "Engine/Core/Utility.h"
#include "Engine/Function/Component/All.h"
#include "Engine/Function/Buffer/All.h"
#include "Engine/Function/Graphics/Texture/All.h"
#include "Engine/Function/Scene.h"
#include "Engine/Function/Graphics/ShaderManager.h"
namespace NoobRenderer
{
    namespace render
    {
        class VoxelGlobalIllumination
        {
        private:
            inline static bool ready{nullptr};
            inline static ShaderProgram::Ptr voxelize_shader{nullptr};
            inline static WriteToTexture::Ptr voxel_vis_rt{nullptr};
            inline static WriteToTexture::Ptr vxgi_rt{nullptr};
            inline static std::array<glm::mat4, 3> voxel_projection_view_mat;
            static void Init(component::VoxelGlobalIllumination &gi);
            static void Voxelize(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi);
            static void LightInjection(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi);
            static void BuildMipmap(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi);
            static void ConeTracing(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi);

        public:
            static void Apply(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi);
        };
    }
}
