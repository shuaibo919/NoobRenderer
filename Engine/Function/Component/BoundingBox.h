#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/Serialization.h"
#include "Engine/Function/Buffer/Vertex.h"
namespace NoobRenderer
{
    namespace component
    {
        struct AABBBoudingBox
        {
            float minx{std::numeric_limits<float>::max()};
            float miny{std::numeric_limits<float>::max()};
            float minz{std::numeric_limits<float>::max()};
            float maxx{std::numeric_limits<float>::min()};
            float maxy{std::numeric_limits<float>::min()};
            float maxz{std::numeric_limits<float>::min()};
            AABBBoudingBox() {}
            AABBBoudingBox(std::vector<glm::vec3> &vertices, glm::mat4 model);
            AABBBoudingBox(std::vector<NoobRenderer::Vertex> &vertices, glm::mat4 model);
            void Merge(AABBBoudingBox &other);
            SERIALIZATION_COMPONENT_ENABLE(AABBBoudingBox, minx, miny, minz, maxx, maxy, maxz)
            SERIALIZATION_FROM_TYPE_ANY(AABBBoudingBox)
        };
    } // namespace component
};
