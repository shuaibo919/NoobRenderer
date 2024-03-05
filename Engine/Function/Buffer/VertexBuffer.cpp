#include "Engine/Function/Buffer/VertexBuffer.h"
using namespace NoobRenderer;

VertexBuffer::VertexBuffer() : BufferBase(), length(0), hasIndexBuffer(false) {}

VertexBuffer::VertexBuffer(size_t _length)
    : BufferBase(), length(_length), hasIndexBuffer(false)
{
    InitBuffer();
}

VertexBuffer::VertexBuffer(std::vector<Vertex> &vertices)
    : BufferBase(), length(vertices.size()), hasIndexBuffer(false)
{
    InitBuffer(vertices, {});
}

VertexBuffer::VertexBuffer(std::vector<Vertex> &vertices, std::vector<unsigned int> indices)
    : BufferBase(), length(vertices.size()), hasIndexBuffer(true)
{
    InitBuffer(vertices, indices);
}

VertexBuffer::VertexBuffer(const VertexBuffer &vertexBuffer)
    : length(vertexBuffer.length), hasIndexBuffer(vertexBuffer.hasIndexBuffer)
{
    if (vertexBuffer.indexBuffer != nullptr)
        indexBuffer = vertexBuffer.indexBuffer;
    id = vertexBuffer.id;
}

VertexBuffer::VertexBuffer(VertexBuffer &&vertexBuffer) noexcept
    : length(vertexBuffer.length), hasIndexBuffer(vertexBuffer.hasIndexBuffer)
{
    if (indexBuffer != nullptr)
        indexBuffer = std::move(vertexBuffer.indexBuffer);
    id = vertexBuffer.id;
}

VertexBuffer &VertexBuffer::operator=(const VertexBuffer &vertexBuffer)
{
    id = vertexBuffer.id;
    length = vertexBuffer.length;
    hasIndexBuffer = vertexBuffer.hasIndexBuffer;
    if (indexBuffer != nullptr)
        indexBuffer = vertexBuffer.indexBuffer;
    return *this;
}

VertexBuffer::~VertexBuffer()
{
    Unbind();
    glDeleteBuffers(1, &id);
}

void VertexBuffer::VertexAttributes()
{
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // texture coordinates attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    // tangent attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
    // bitangent attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));
    // ids attribute
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, Vertex::MAX_BONE_INFLUENCE, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs));
    // weights attribute
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, Vertex::MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_Weights));
}

void VertexBuffer::InitBuffer(std::vector<Vertex> &vertices, std::vector<unsigned int> indices)
{

    // Load vertices
    unsigned int index = 0;

    // Vertex buffer
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Index Buffer
    if (hasIndexBuffer && !indices.empty())
        indexBuffer = std::make_shared<IndexBuffer>(indices);

    // Vertex Attributes
    VertexAttributes();

    // Unbind VBO
    Unbind();
}

void VertexBuffer::InitBuffer()
{

    // Vertex buffer
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // Vertex Attributes
    VertexAttributes();

    // Unbind VBO
    Unbind();
}

std::vector<Vertex> VertexBuffer::GetVertices()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    float *ptr = (float *)glMapNamedBuffer(id, GL_READ_WRITE);
    if (ptr == nullptr) return {};

    std::vector<Vertex> vertices;
    auto step = sizeof(Vertex) /4;
    for (int i = 0; i < length; i++)
    {
        // vec3 - 顶点位置 Position vec3 - 顶点法线 Normal vec2 - 纹理坐标 TexCoords vec3 - tangent Tangent vec3 - bitangent Bitangent
        Vertex v{};
        v.Position.x =  ptr[i * step];
        v.Position.y = ptr[i* step + 1];
        v.Position.z = ptr[i * step + 2];

        v.Normal.x = ptr[i * step + 3];
        v.Normal.y = ptr[i * step + 4];
        v.Normal.z = ptr[i * step + 5];

        v.TexCoords.x = ptr[i * step + 6];
        v.TexCoords.y = ptr[i * step + 7];

        v.Tangent.x = ptr[i * step + 8];
        v.Tangent.y = ptr[i * step + 9];
        v.Tangent.z = ptr[i * step + 10];

        v.Bitangent.x = ptr[i * step + 11];
        v.Bitangent.y = ptr[i * step + 12];
        v.Bitangent.z = ptr[i * step + 13];

        vertices.push_back(v);
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vertices;
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}