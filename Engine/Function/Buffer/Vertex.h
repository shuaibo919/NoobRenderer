#pragma once
#include "Engine/Function/Buffer/BufferBase.h"

namespace NoobRenderer
{

    /**
     * @brief
     * vec3 - 顶点位置 Position
     * vec3 - 顶点法线 Normal
     * vec2 - 纹理坐标 TexCoords
     * vec3 - tangent Tangent
     * vec3 - bitangent Bitangent
     */
    struct Vertex
    {
        static const int MAX_BONE_INFLUENCE = 4;
        glm::vec3 Position;                  // 顶点位置
        glm::vec3 Normal;                    // 顶点法线
        glm::vec2 TexCoords;                 // 纹理坐标
        glm::vec3 Tangent;                   // tangent
        glm::vec3 Bitangent;                 // bitangent
        int m_BoneIDs[MAX_BONE_INFLUENCE];   // bone indexes which will influence this vertex
        float m_Weights[MAX_BONE_INFLUENCE]; // weights from each bone
    };

}
