#pragma once
#include "Engine/Core/Serialization.h"
#include "Engine/Function/Component/All.h"
#include "Engine/Function/Entity.h"

namespace NoobRenderer
{
    namespace Registeration
    {
        inline static void Init()
        {
            using namespace NoobRenderer;
            ComponentRegisterationMap::Register<component::NameComponent>("NameComponent");
            ComponentRegisterationMap::Register<component::ActiveComponent>("ActiveComponent");
            ComponentRegisterationMap::Register<component::Light>("Light");
            ComponentRegisterationMap::Register<component::PointLight>("PointLight");
            ComponentRegisterationMap::Register<component::DirectionalLight>("DirectionalLight");
            ComponentRegisterationMap::Register<component::AreaLight>("AreaLight");
            ComponentRegisterationMap::Register<component::MeshData>("MeshData");
            ComponentRegisterationMap::Register<component::Transform>("Transform");
            ComponentRegisterationMap::Register<component::Camera>("Camera");
            ComponentRegisterationMap::Register<component::IBLCubeMap>("IBLCubeMap");
            ComponentRegisterationMap::Register<component::ScreenSpaceReflection>("ScreenSpaceReflection");
            ComponentRegisterationMap::RegisterPreCalculation<component::IBLCubeMap>(&component::IBLCubeMap::ConductPreCalculation);
            // Todo: LTC PreCalculation
            // ComponentRegisterationMap::RegisterPreCalculation<component::AreaLight>()
        }

    }
}
