#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Function/Buffer/All.h"
namespace NoobRenderer
{

    class AnimationSystem : public SubSystem
    {

    public:
        AnimationSystem() = default;
        void Init() override;
        void Update() override;
    };
} // namespace NoobRenderer
