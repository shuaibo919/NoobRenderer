#pragma once
#include "Engine/Core/Base.hpp"
#include "Third/nlohmann/json.hpp"
#include "Third/entt/entt.hpp"

namespace NoobRenderer
{
    namespace serialization
    {
        using BindComponentFunc = std::function<void(entt::entity &, std::shared_ptr<entt::registry> &, nlohmann::json &)>;
        using FromVoidTypeFunc = std::function<nlohmann::json(void *)>;
    }
}

#define DESERILIZING_TO_ENTITY(Type)                                                                             \
    static void BindComponentFromJson(entt::entity &id, std::shared_ptr<entt::registry> &reg, nlohmann::json &j) \
    {                                                                                                            \
        Type t = j;                                                                                              \
        reg->emplace<Type>(id, t);                                                                               \
    }

#define SERIALIZATION_FROM_TYPE_ANY(Type)                    \
    inline static nlohmann::json FromVoidType(void *typeAny) \
    {                                                        \
        return (*static_cast<Type *>(typeAny));              \
    }

#define SERIALIZATION_NORMAL_ENABLE(Type, ...) NLOHMANN_DEFINE_TYPE_INTRUSIVE(Type, __VA_ARGS__)

#define SERIALIZATION_COMPONENT_ENABLE(Type, ...)  \
    SERIALIZATION_NORMAL_ENABLE(Type, __VA_ARGS__) \
    DESERILIZING_TO_ENTITY(Type)

namespace glm
{
    void to_json(nlohmann::json &j, const glm::vec2 &P);
    void from_json(const nlohmann::json &j, glm::vec2 &P);
    void to_json(nlohmann::json &j, const glm::vec3 &P);
    void from_json(const nlohmann::json &j, glm::vec3 &P);
    void to_json(nlohmann::json &j, const glm::vec4 &P);
    void from_json(const nlohmann::json &j, glm::vec4 &P);
    void to_json(nlohmann::json &j, const glm::mat4 &P);
    void from_json(const nlohmann::json &j, glm::mat4 &P);
}