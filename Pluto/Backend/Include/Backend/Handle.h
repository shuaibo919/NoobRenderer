#ifndef PLUTO_HANDLE_H
#define PLUTO_HANDLE_H

#include <type_traits> // FIXME: STL headers are not allowed in public headers

#include <stdint.h>

namespace pluto::backend {

struct RHIBufferObject;
struct RHIFence;
struct RHIIndexBuffer;
struct RHIProgram;
struct RHIRenderPrimitive;
struct RHIRenderTarget;
struct RHISamplerGroup;
struct RHIStream;
struct RHISwapChain;
struct RHITexture;
struct RHITimerQuery;
struct RHIVertexBufferInfo;
struct RHIVertexBuffer;


//! \privatesection

class HandleBase {
public:
    using HandleId = uint32_t;
    static constexpr const HandleId nullid = HandleId{ UINT32_MAX };

    constexpr HandleBase() noexcept: object(nullid) {}

    // whether this Handle is initialized
    explicit operator bool() const noexcept { return object != nullid; }

    // clear the handle, this doesn't free associated resources
    void clear() noexcept { object = nullid; }

    // get this handle's handleId
    HandleId getId() const noexcept { return object; }

    // initialize a handle, for internal use only.
    explicit HandleBase(HandleId id) noexcept : object(id) {
       // assert_invariant(object != nullid); // usually means an uninitialized handle is used
    }

protected:
    HandleBase(HandleBase const& rhs) noexcept = default;
    HandleBase& operator=(HandleBase const& rhs) noexcept = default;

    HandleBase(HandleBase&& rhs) noexcept
            : object(rhs.object) {
        rhs.object = nullid;
    }

    HandleBase& operator=(HandleBase&& rhs) noexcept {
        if (this != &rhs) {
            object = rhs.object;
            rhs.object = nullid;
        }
        return *this;
    }

private:
    HandleId object;
};


template<typename T>
struct Handle : public HandleBase {

    Handle() noexcept = default;

    Handle(Handle const& rhs) noexcept = default;
    Handle(Handle&& rhs) noexcept = default;

    Handle& operator=(Handle const& rhs) noexcept = default;
    Handle& operator=(Handle&& rhs) noexcept = default;

    explicit Handle(HandleId id) noexcept : HandleBase(id) { }

    // compare handles of the same type
    bool operator==(const Handle& rhs) const noexcept { return getId() == rhs.getId(); }
    bool operator!=(const Handle& rhs) const noexcept { return getId() != rhs.getId(); }
    bool operator<(const Handle& rhs) const noexcept { return getId() < rhs.getId(); }
    bool operator<=(const Handle& rhs) const noexcept { return getId() <= rhs.getId(); }
    bool operator>(const Handle& rhs) const noexcept { return getId() > rhs.getId(); }
    bool operator>=(const Handle& rhs) const noexcept { return getId() >= rhs.getId(); }

    // type-safe Handle cast
    template<typename B, typename = std::enable_if_t<std::is_base_of<T, B>::value> >
    Handle(Handle<B> const& base) noexcept : HandleBase(base) { }
};

using BufferObjectHandle     = Handle<RHIBufferObject>;
using FenceHandle            = Handle<RHIFence>;
using IndexBufferHandle      = Handle<RHIIndexBuffer>;
using ProgramHandle          = Handle<RHIProgram>;
using RenderPrimitiveHandle  = Handle<RHIRenderPrimitive>;
using RenderTargetHandle     = Handle<RHIRenderTarget>;
using SamplerGroupHandle     = Handle<RHISamplerGroup>;
using StreamHandle           = Handle<RHIStream>;
using SwapChainHandle        = Handle<RHISwapChain>;
using TextureHandle          = Handle<RHITexture>;
using TimerQueryHandle       = Handle<RHITimerQuery>;
using VertexBufferHandle     = Handle<RHIVertexBuffer>;
using VertexBufferInfoHandle = Handle<RHIVertexBufferInfo>;

} 

#endif // PLUTO_HANDLE_H