#pragma once
#include "Engine/Function/Buffer/All.h"
#include "Engine/Function/Utility/RayCasting.h"
#include "Engine/Function/Graphics/Texture/ColorTexture.h"
#include "Engine/Function/Graphics/Material/All.h"

namespace NoobRenderer
{

    enum class DrawPrimitive : GLuint
    {
        Default = 0,
        Triangle = GL_TRIANGLES,
        Line = GL_LINES,
        LineStrip = GL_LINE_STRIP,
        LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
        TriangleStrip = GL_TRIANGLE_STRIP
    };
    inline static std::string GetPrimitiveString(DrawPrimitive dp)
    {
        switch (dp)
        {
        case DrawPrimitive::Triangle:
            return "Triangle";
        case DrawPrimitive::Line:
            return "Line";
        case DrawPrimitive::TriangleStrip:
            return "TriangleStrip";
        case DrawPrimitive::LineStrip:
            return "LineStrip";
        case DrawPrimitive::LineStripAdjacency:
            return "LineStripAdjacency";
        default:
            return "Unknown";
        }
    }
    /**
     * @brief Mesh 网格
     */
    class Mesh
    {
    private:
        VertexArray::Ptr m_vertexArray{nullptr};
        VertexBuffer::Ptr m_vertexBuffer{nullptr};
        Material::Ptr m_material{nullptr};
        DrawPrimitive m_primitive{DrawPrimitive::Triangle};
        size_t m_vertexLength, m_indicesLength;
        void InitMesh(size_t length);
        void InitMesh(std::vector<Vertex> &vertices);
        void InitMesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);

    public:
        using Ptr = std::shared_ptr<Mesh>;
        Mesh(size_t length);
        Mesh(std::vector<Vertex> &vertices);
        Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
        Mesh(const Mesh &mesh);
        Mesh(Mesh &&mesh) noexcept;
        Mesh() = default;
        void Bind();
        void Unbind();
        void Draw();
        void Draw(DrawPrimitive dp);
        EngineUtility::RayCastingRecord IsPicking(glm::vec3 &camera_pos, glm::mat4 &transform, glm::vec3 &mouse_ray);
        inline DrawPrimitive GetPrimitive() const { return m_primitive; }
        inline void SetPrimitive(DrawPrimitive primitive) { m_primitive = primitive; }

    public:
        inline void SetMaterial(const Material::Ptr &material) { m_material = material; }
        inline Material::Ptr &GetMaterial() { return m_material; }
        inline size_t GetIndiceLength() const { return m_indicesLength; }
        inline size_t GetVertexLength() const { return m_vertexLength; }
    };
    using MeshIndex = int;
}