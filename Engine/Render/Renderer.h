#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"
#include "Engine/Core/Utility.h"
#include "Engine/Function/Buffer/All.h"
#include "Engine/Function/SceneManager.h"
#include "Engine/Function/Graphics/MeshManager.h"
#include "Engine/Function/Graphics/ShaderManager.h"
#include "Engine/Function/Graphics/Texture/All.h"
#include "Engine/Function/Graphics/Material/All.h"
#include "Engine/Function/Component/All.h"
#include "Engine/Function/Utility/Helpers.h"
#include "Engine/Render/Pass/RenderPass.h"

namespace NoobRenderer
{
    namespace render
    {
        enum class OperationSetMatUniform
        {
            None,
            Float,
            Vec3
        };
        /**
         * @brief 渲染器，虚函数Create，Resize, Init, Update
         */
        class Renderer : public std::enable_shared_from_this<Renderer>
        {
        public:
            struct Size
            {
                unsigned int width;
                unsigned int height;
            };
            Renderer(unsigned int width, unsigned int height) : RenderViewPort{width, height} {}
            virtual void Resize(unsigned int width, unsigned int height){};
            virtual void Init(){};
            virtual void Update(){};

        protected:
            GLint id{-1};

        public:
            using Ptr = std::shared_ptr<Renderer>;
            RenderStorage RTS;
            Size RenderViewPort;

        public:
            inline Size GetRenderSize() const { return RenderViewPort; }
            inline GLint GetRenderResult() { return id; }
        };
    }
}
