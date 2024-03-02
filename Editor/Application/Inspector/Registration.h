#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "Engine/Function/Component/All.h"
#include "Engine/Function/Entity.h"

namespace NoobRenderer
{
    namespace inspection
    {
        using FromVoidType = std::function<void(NoobRenderer::Entity::Ptr &, void *)>;
        template <typename T>
        inline static void InspectComponent(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
        }

        class ComponentUIMap
        {
        private:
            inline static std::unordered_map<entt::id_type, inspection::FromVoidType> idtype_to_uiFunc;

        public:
            template <typename T>
            inline static void Register()
            {
                idtype_to_uiFunc[entt::type_id<T>().hash()] = &inspection::InspectComponent<T>;
            }
            inline static void Inspect(NoobRenderer::Entity::Ptr &entity, entt::id_type id, void *&typeAny)
            {
                if (idtype_to_uiFunc.find(id) == idtype_to_uiFunc.end())
                    return;
                idtype_to_uiFunc[id](entity, typeAny);
            }
        };

    }
}