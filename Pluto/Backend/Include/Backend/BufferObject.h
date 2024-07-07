#ifndef PLUTO_BACKEND_VERTEX_H
#define PLUTO_BACKEND_VERTEX_H

#include "Backend/Backend.h"
#include "Backend/Utils.h"
#include "Backend/BackendEnums.h"
#include "Backend/BufferDescriptor.h"
#include <cstdint>

namespace pluto::backend
{
    class BufferObject
    {
        struct CreateProperties;

    public:
        using BufferDescriptor = backend::BufferDescriptor;

        class Create : public CreateBase<CreateProperties>
        {
            friend struct CreateProperties;
            friend class BufferObjectHelper;

        public:
            Create() noexcept = default;
            Create(Create const &rhs) noexcept = default;
            Create(Create &&rhs) noexcept = default;
            ~Create() noexcept = default;
            Create &operator=(Create const &rhs) noexcept = default;
            Create &operator=(Create &&rhs) noexcept = default;

            Create &size(uint32_t byteCount) noexcept;
            Create &bindingType(BufferObjectBinding bindingType) noexcept;

            BufferObject *build(RHILayer *rhi);

        private:
            friend class FBufferObject;
        };

        void setBuffer(BufferDescriptor &&buffer, uint32_t byteOffset = 0);

        size_t getByteCount() const noexcept;

    protected:
        // prevent heap allocation
        ~BufferObject() = default;
    };
}

#endif