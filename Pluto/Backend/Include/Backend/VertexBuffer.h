#ifndef PLUTO_BACKEND_VERTEX_H
#define PLUTO_BACKEND_VERTEX_H

#include "Backend/Utils.h"
#include "Backend/BufferObject.h"
#include "Backend/BufferDescriptor.h"
#include <cstdint>

namespace pluto::backend
{
    class BufferObject;
    class AttributeType
    {
    };

    class VertexAttribute
    {
    };

    class VertexBuffer
    {
        struct CreateProperties;

    public:
        struct Create final : public CreateBase<CreateProperties>
        {
            friend struct CreateProperties;
            Create() noexcept = default;
            Create(Create const &rhs) noexcept = default;
            Create(Create &&rhs) noexcept = default;
            ~Create() noexcept = default;
            Create &operator=(Create const &rhs) noexcept = default;
            Create &operator=(Create &&rhs) noexcept = default;

            Create &bufferCount(uint8_t bufferCount) noexcept;
            Create &vertexCount(uint32_t vertexCount) noexcept;
            Create &enableBufferObjects(bool enabled = true) noexcept;
            Create &attribute(VertexAttribute attribute, uint8_t bufferIndex,
                              AttributeType attributeType,
                              uint32_t byteOffset = 0, uint8_t byteStride = 0) noexcept;
            Create &normalized(VertexAttribute attribute, bool normalize = true) noexcept;
            Create &advancedSkinning(bool enabled) noexcept;

            VertexBuffer *build();
        };

        size_t getVertexCount() const noexcept;

        void setBufferAt(uint8_t bufferIndex, BufferDescriptor &&buffer,
                         uint32_t byteOffset = 0);

        void setBufferObjectAt(uint8_t bufferIndex, BufferObject const *bufferObject);
    };

}

#endif