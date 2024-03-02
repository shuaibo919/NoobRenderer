#include "Engine/Function/Graphics/Mesh.h"
using namespace NoobRenderer;
Mesh::Mesh(size_t length)
    : m_vertexLength(length), m_indicesLength(0)
{
    InitMesh(length);
    m_material = std::make_shared<Material>();
}

Mesh::Mesh(std::vector<Vertex> &vertices)
    : m_vertexLength(vertices.size()), m_indicesLength(0)
{
    InitMesh(vertices);
    m_material = std::make_shared<Material>();
}

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices)
    : m_vertexLength(vertices.size()), m_indicesLength(indices.size())
{
    InitMesh(vertices, indices);
    m_material = std::make_shared<Material>();
}

Mesh::Mesh(const Mesh &mesh)
    : m_vertexArray(mesh.m_vertexArray), m_vertexBuffer(mesh.m_vertexBuffer), m_primitive(mesh.m_primitive),
      m_vertexLength(mesh.m_vertexLength), m_indicesLength(mesh.m_indicesLength), m_material(GetMaterialByCopyCtor(mesh.m_material))
{
}

Mesh::Mesh(Mesh &&mesh) noexcept
    : m_vertexArray(std::move(mesh.m_vertexArray)), m_vertexBuffer(std::move(mesh.m_vertexBuffer)), m_primitive(mesh.m_primitive),
      m_vertexLength(mesh.m_vertexLength), m_indicesLength(mesh.m_indicesLength), m_material(std::move(mesh.m_material))
{
}
void Mesh::InitMesh(size_t length)
{
    m_vertexArray = std::make_shared<VertexArray>();
    m_vertexBuffer = std::make_shared<VertexBuffer>(length);
    Unbind();
}
void Mesh::InitMesh(std::vector<Vertex> &vertices)
{
    m_vertexArray = std::make_shared<VertexArray>();
    m_vertexBuffer = std::make_shared<VertexBuffer>(vertices);
    Unbind();
}
void Mesh::InitMesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices)
{
    m_vertexArray = std::make_shared<VertexArray>();
    m_vertexBuffer = std::make_shared<VertexBuffer>(vertices, indices);
    Unbind();
}
void Mesh::Bind()
{
    if (m_vertexArray != nullptr && m_vertexBuffer != nullptr)
    {
        m_vertexArray->Bind();
    }
}
void Mesh::Unbind()
{
    if (m_vertexArray != nullptr && m_vertexBuffer != nullptr)
        m_vertexArray->Unbind();
}
void Mesh::Draw()
{
    Bind();
    if (!m_vertexBuffer->HasIndexBuffer())
    {
        glDrawArrays(static_cast<GLuint>(this->m_primitive), 0, static_cast<GLsizei>(m_vertexLength));
    }
    else
    {
        glDrawElements(static_cast<GLuint>(this->m_primitive), static_cast<GLsizei>(m_indicesLength), GL_UNSIGNED_INT, 0);
    }
    Unbind();
}
void Mesh::Draw(DrawPrimitive dp)
{
    Bind();
    if (!m_vertexBuffer->HasIndexBuffer())
    {
        glDrawArrays(static_cast<GLuint>(dp), 0, static_cast<GLsizei>(m_vertexLength));
    }
    else
    {
        glDrawElements(static_cast<GLuint>(dp), static_cast<GLsizei>(m_indicesLength), GL_UNSIGNED_INT, 0);
    }
    Unbind();
}

EngineUtility::RayCastingRecord Mesh::IsPicking(glm::vec3 &camera_pos, glm::mat4 &transform, glm::vec3 &mouse_ray)
{
    if (m_vertexArray == nullptr || m_vertexBuffer == nullptr)
        return EngineUtility::RayCastingRecord();

    // 得到顶点和索引缓冲数据
    auto vertices_data = m_vertexBuffer->GetVertices();
    std::vector<unsigned int> indices_data;
    if (m_vertexBuffer->HasIndexBuffer())
        indices_data = m_vertexBuffer->GetIndexBuffer()->GetIndices();
    else
    {
        for (unsigned int i = 0; i < m_vertexLength; ++i)
            indices_data.emplace_back(i);
    }
    for (size_t i = 0; i < indices_data.size(); i += 3)
    {
        // mesh数据中的位置并不是实际渲染的位置,需要先经过Model变换
        auto p1 = indices_data[i], p2 = indices_data[i + 1], p3 = indices_data[i + 2];
        glm::vec3 v1 = transform * glm::vec4(vertices_data[p1].Position, 1.0f);
        glm::vec3 v2 = transform * glm::vec4(vertices_data[p2].Position, 1.0f);
        glm::vec3 v3 = transform * glm::vec4(vertices_data[p3].Position, 1.0f);
        // 计算参数t: intersection(x,y,z) = ray * t + camepra_pos;
        glm::vec3 dir1 = glm::vec3(v2 - v1);
        glm::vec3 dir2 = glm::vec3(v3 - v1);
        glm::vec3 normal = glm::normalize(glm::cross(dir1, dir2));

        glm::vec3 pvec = glm::cross(mouse_ray, dir2);
        float det = glm::dot(dir1, pvec);
        if (abs(det) < 0.0001f)
        {
            continue;
        }
        float inv_det = 1.0f / det;
        glm::vec3 tvec = camera_pos - v1;
        float u = glm::dot(tvec, pvec) * inv_det;
        if (u < 0.0f || u > 1.0f)
        {
            continue;
        }
        glm::vec3 qvec = glm::cross(tvec, dir1);
        float v = glm::dot(mouse_ray, qvec) * inv_det;
        if (v < 0.0f || u + v > 1.0f)
        {
            continue;
        }
        float t = glm::dot(dir2, qvec) * inv_det;
        if (t > 0.0001f)
        {
            glm::vec3 noraml = glm::normalize(glm::cross(dir1, dir2));
            glm::vec3 hit = camera_pos + t * mouse_ray;
            return EngineUtility::RayCastingRecord(t, hit, noraml);
        }
    }
    return EngineUtility::RayCastingRecord();
}

// EngineUtility::RayCastingRecord MeshList::IsPicking(glm::vec3 &camera_pos, glm::mat4 &transform, glm::vec3 &mouse_ray)
// {
//     auto _record = EngineUtility::RayCastingRecord();
//     for (auto &mesh : m_meshes)
//     {
//         auto tmp = mesh->IsPicking(camera_pos, transform, mouse_ray);
//         if (tmp.GetHitted() && tmp.GetRayFactor() < _record.GetRayFactor())
//         {
//             _record = tmp;
//         }
//     }
//     return _record;
// }
