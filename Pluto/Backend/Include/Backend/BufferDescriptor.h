#ifndef PLUTO_BUFFER_DESCRIPTOR_H
#define PLUTO_BUFFER_DESCRIPTOR_H

#include <cstddef>

namespace pluto::backend {

class CallbackHandler;

class BufferDescriptor {
public:
    using Callback = void(*)(void* buffer, size_t size, void* user);

    BufferDescriptor() noexcept = default;

    ~BufferDescriptor() noexcept 
    {
        if (mCallback) mCallback(buffer, size, mUser);
    }

    // 禁止拷贝
    BufferDescriptor(const BufferDescriptor& rhs) = delete;
    BufferDescriptor& operator=(const BufferDescriptor& rhs) = delete;

    // 允许移动
    BufferDescriptor(BufferDescriptor&& rhs) noexcept
        : buffer(rhs.buffer), size(rhs.size),
          mCallback(rhs.mCallback), mUser(rhs.mUser), mHandler(rhs.mHandler) {
            rhs.buffer = nullptr;
            rhs.mCallback = nullptr;
    }

    BufferDescriptor& operator=(BufferDescriptor&& rhs) noexcept
    {
        if (this != &rhs) {
            buffer = rhs.buffer;
            size = rhs.size;
            mCallback = rhs.mCallback;
            mUser = rhs.mUser;
            mHandler = rhs.mHandler;
            rhs.buffer = nullptr;
            rhs.mCallback = nullptr;
        }
        return *this;
    }

    BufferDescriptor(void const* buffer, size_t size,
            Callback callback = nullptr, void* user = nullptr) noexcept
            : buffer(const_cast<void*>(buffer)), size(size), mCallback(callback), mUser(user) {}

    BufferDescriptor(void const* buffer, size_t size,
            CallbackHandler* handler, Callback callback, void* user = nullptr) noexcept
            : buffer(const_cast<void*>(buffer)), size(size),
            mCallback(callback), mUser(user), mHandler(handler) {}

    template<typename T, void(T::*method)(void const*, size_t)>
    static BufferDescriptor make(void const* buffer, size_t size, T* data,
            CallbackHandler* handler = nullptr) noexcept 
    {
        return {
                buffer, size,
                handler, [](void* b, size_t s, void* u) {
                    (static_cast<T*>(u)->*method)(b, s);
                }, data
        };
    }

    template<typename T>
    static BufferDescriptor make(void const* buffer, size_t size, T&& functor,
            CallbackHandler* handler = nullptr) noexcept 
    {
        return {
                buffer, size,
                handler, [](void* b, size_t s, void* u) {
                    T* const that = static_cast<T*>(u);
                    that->operator()(b, s);
                    delete that;
                },
                new T(std::forward<T>(functor))
        };
    }

    void setCallback(Callback callback, void* user = nullptr) noexcept 
    {
        this->mCallback = callback;
        this->mUser = user;
        this->mHandler = nullptr;
    }

    void setCallback(CallbackHandler* handler, Callback callback, void* user = nullptr) noexcept 
    {
        mCallback = callback;
        mUser = user;
        mHandler = handler;
    }

    bool hasCallback() const noexcept 
    { 
        return mCallback != nullptr; 
    }

    Callback getCallback() const noexcept 
    {
        return mCallback;
    }

    CallbackHandler* getHandler() const noexcept 
    {
        return mHandler;
    }

    void* getUser() const noexcept 
    {
        return mUser;
    }

    void* buffer = nullptr;
    size_t size = 0;

private:
    Callback mCallback = nullptr;
    void* mUser = nullptr;
    CallbackHandler* mHandler = nullptr;
};

} // namespace filament::backend

#endif // PLUTO_BUFFER_DESCRIPTOR_H