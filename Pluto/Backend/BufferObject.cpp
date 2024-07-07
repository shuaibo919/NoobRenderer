#include "Backend/BufferObject.h"
#include "Backend/BackendEnums.h"
namespace pluto::backend
{
    struct BufferObject::CreateProperties
    {
        BufferObjectBinding mBindingType = BufferObjectBinding::Vertex;
        uint32_t mByteCount = 0;
    };

    BufferObject::Create &BufferObject::Create::size(uint32_t byteCount) noexcept
    {
        mImpl->mByteCount = byteCount;
        return *this;
    }

    BufferObject::Create &BufferObject::Create::bindingType(BufferObjectBinding bindingType) noexcept
    {
        mImpl->mBindingType = bindingType;
        return *this;
    }

    BufferObject *BufferObject::Create::build(RHILayer *rhi)
    {
        // return rhi->createBufferObject(mImpl->mBindingType, mImpl->mByteCount).release();
        return nullptr;
    }
}