#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Function/Buffer/All.h"
namespace NoobRenderer
{

    class RenderPassContext
    {
    public:
        RenderPassContext() = default;
        inline FrameBuffer::Ptr GetFrameBuffer() { return m_BackBuffer; }
    protected:
        FrameBuffer::Ptr m_BackBuffer = nullptr;
    };

    class GraphicSystem : public SubSystem
    {
        enum class RenderMode
        {
            Forward = 0,
            Derived,
        };

    public:
        GraphicSystem() = default;
        void Init() override;
        void Update() override;

    private:
        FrameBuffer::Ptr m_final_target{nullptr};
    };
} // namespace NoobRenderer
