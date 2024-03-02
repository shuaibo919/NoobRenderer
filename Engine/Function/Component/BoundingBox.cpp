#include "Engine/Function/Component/BoundingBox.h"
#include "Engine/Core/Utility.h"
using namespace NoobRenderer::component;
AABBBoudingBox::AABBBoudingBox(std::vector<glm::vec3> &vertices, glm::mat4 model)
{
    for (auto position : vertices)
    {
        glm::vec4 world_pos = model * glm::vec4(position, 1.0f);
        minx = EngineUtility::Min(minx, world_pos.x);
        miny = EngineUtility::Min(minx, world_pos.y);
        minz = EngineUtility::Min(minx, world_pos.z);
        maxx = EngineUtility::Max(maxx, world_pos.x);
        maxy = EngineUtility::Max(maxy, world_pos.y);
        maxz = EngineUtility::Max(maxz, world_pos.z);
    }
}
AABBBoudingBox::AABBBoudingBox(std::vector<NoobRenderer::Vertex> &vertices, glm::mat4 model)
{
    for (auto vertex : vertices)
    {
        glm::vec4 world_pos = model * glm::vec4(vertex.Position, 1.0f);
        minx = EngineUtility::Min(minx, world_pos.x);
        miny = EngineUtility::Min(minx, world_pos.y);
        minz = EngineUtility::Min(minx, world_pos.z);
        maxx = EngineUtility::Max(maxx, world_pos.x);
        maxy = EngineUtility::Max(maxy, world_pos.y);
        maxz = EngineUtility::Max(maxz, world_pos.z);
    }
}
void AABBBoudingBox::Merge(AABBBoudingBox &other)
{
    minx = EngineUtility::Min(minx, other.minx);
    miny = EngineUtility::Min(miny, other.miny);
    minz = EngineUtility::Min(minz, other.minz);
    maxx = EngineUtility::Max(maxx, other.maxx);
    maxy = EngineUtility::Max(maxy, other.maxy);
    maxz = EngineUtility::Max(maxz, other.maxz);
}