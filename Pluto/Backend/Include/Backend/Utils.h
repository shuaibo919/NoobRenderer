#ifndef PLUTO_UTILS_H
#define PLUTO_UTILS_H

template <typename T>
class PrivateImplementation
{
public:
    // none of these methods must be implemented inline because it's important that their
    // implementation be hidden from the public headers.
    template <typename... args>
    explicit PrivateImplementation(args &&...) noexcept;
    PrivateImplementation() noexcept;
    ~PrivateImplementation() noexcept;
    PrivateImplementation(PrivateImplementation const &rhs) noexcept;
    PrivateImplementation &operator=(PrivateImplementation const &rhs) noexcept;

    // move ctor and copy operator can be implemented inline and don't need to be exported
    PrivateImplementation(PrivateImplementation &&rhs) noexcept : mImpl(rhs.mImpl) { rhs.mImpl = nullptr; }
    PrivateImplementation &operator=(PrivateImplementation &&rhs) noexcept
    {
        auto temp = mImpl;
        mImpl = rhs.mImpl;
        rhs.mImpl = temp;
        return *this;
    }

protected:
    T *mImpl = nullptr;
    inline T *operator->() noexcept { return mImpl; }
    inline T const *operator->() const noexcept { return mImpl; }
};

template <typename T>
using CreateBase = PrivateImplementation<T>;

#endif