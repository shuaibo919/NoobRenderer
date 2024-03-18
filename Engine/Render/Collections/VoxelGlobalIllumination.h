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
            inline static bool ready{false};
            inline static ShaderProgram::Ptr voxelize_shader{nullptr};
            inline static ShaderProgram::Ptr voxel_face_shader{nullptr};
            inline static ShaderProgram::Ptr voxel_vis_shader{nullptr};
            inline static Texture3D::Ptr voxel_albedo{nullptr};
            inline static WriteToTexture::Ptr voxel_vis_rt{nullptr};
            inline static WriteToTexture::Ptr vxgi_rt{nullptr};
            inline static WriteToTexture::Ptr vxgi_visualize_rt{nullptr};
            inline static std::array<glm::mat4, 3> voxel_projection_view_mat;
            static void Init(component::VoxelGlobalIllumination &gi);
            static void Voxelize(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi);
            static void LightInjection(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi);
            static void BuildMipmap(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi);
            static void ConeTracing(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi);

        public:
            inline static glm::vec3 eye = glm::vec3(0.0f, 0.0f, 0.0f);
            inline static glm::vec3 up = glm::vec3(0.0f, 0.0f, 0.0f);
            static void Apply(Scene::Ptr &scene, component::VoxelGlobalIllumination &gi);
            static void DrawVoxelScene(Scene::Ptr &scene, glm::ivec2 viewport_size, component::VoxelGlobalIllumination &gi);
            static WriteToTexture::Ptr &GetDrawVoxelScene();
            static WriteToTexture::Ptr &GetVoxelizeRT();
            static void ClearALLDebugRT();
        };
    }
}
