#include "Engine/Core/Serialization.h"
namespace glm
{
    void to_json(nlohmann::json &j, const glm::vec2 &P)
    {
        j = {{"x", P.x}, {"y", P.y}};
    };

    void from_json(const nlohmann::json &j, glm::vec2 &P)
    {
        P.x = j.at("x").get<float>();
        P.y = j.at("y").get<float>();
    }

    void to_json(nlohmann::json &j, const glm::vec3 &P)
    {
        j = {{"x", P.r}, {"y", P.g}, {"z", P.b}};
    }

    void from_json(const nlohmann::json &j, glm::vec3 &P)
    {
        P.r = j.at("x").get<float>();
        P.g = j.at("y").get<float>();
        P.b = j.at("z").get<float>();
    }

    void to_json(nlohmann::json &j, const glm::vec4 &P)
    {
        auto _array = nlohmann::json::array();
        _array.emplace_back(P.r);
        _array.emplace_back(P.g);
        _array.emplace_back(P.b);
        _array.emplace_back(P.a);
        j = {{"data", _array}};
    };

    void from_json(const nlohmann::json &j, glm::vec4 &P)
    {
        P.r = j.at("data")[0].get<float>();
        P.g = j.at("data")[1].get<float>();
        P.b = j.at("data")[2].get<float>();
        P.a = j.at("data")[3].get<float>();
    }

    void to_json(nlohmann::json &j, const glm::mat4 &P)
    {
        j = {{"0", P[0]}, {"1", P[1]}, {"2", P[2]}, {"3", P[3]}};
    }

    void from_json(const nlohmann::json &j, glm::mat4 &P)
    {
        P[0] = j.at("0").get<glm::vec4>();
        P[1] = j.at("1").get<glm::vec4>();
        P[2] = j.at("2").get<glm::vec4>();
        P[3] = j.at("3").get<glm::vec4>();
    }
}