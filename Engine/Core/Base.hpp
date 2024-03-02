#pragma once
#include <string>
#include <string_view>
#include <memory>
#include <array>
#include <vector>
#include <unordered_map>
#include <functional>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

namespace NoobRenderer
{
    template <typename T>
    class Singleton
    {
    public:
        static T &Instance();

        Singleton(const Singleton &) = delete;
        Singleton &operator=(const Singleton) = delete;

    protected:
        struct token
        {
        };
        Singleton() {}
    };

    template <typename T>
    T &Singleton<T>::Instance()
    {
        static std::unique_ptr<T> _instance{new T{token{}}};
        return *_instance;
    }

    /// @brief 子系统 虚函数:Init(),Update()
    class SubSystem : public std::enable_shared_from_this<SubSystem>
    {
    public:
        SubSystem() = default;
        virtual ~SubSystem()
        {
        }

        virtual void Init() = 0;
        virtual void Update() = 0;
    };
}
