#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/Serialization.h"
#include "Engine/Core/Utility.h"
#include "Engine/Function/Graphics/Texture/Texture.h"
namespace NoobRenderer
{

    /// @brief 通用材质纹理
    struct MaterialTexture
    {
        using Ptr = std::shared_ptr<MaterialTexture>;
        OrdinaryTexture::Ptr texture2d{nullptr};
        glm::vec3 factor;
        MaterialTexture() : texture2d(nullptr), factor(glm::vec3(0.0f)) {}
        MaterialTexture(float val) { factor = glm::vec3(val, -1, -1); }
        MaterialTexture(glm::vec3 val) { factor = val; }
        MaterialTexture(const MaterialTexture::Ptr &_matex) : factor(_matex->factor)
        {
            if (_matex == nullptr)
                return;
            if (_matex->texture2d != nullptr)
                texture2d = std::make_shared<OrdinaryTexture>(*(_matex->texture2d));
        }
        MaterialTexture(MaterialTexture::Ptr &&_matex) noexcept: factor(std::move(_matex->factor))
        {
            if (_matex == nullptr)
                return;
            if (_matex->texture2d != nullptr)
                texture2d = std::make_shared<OrdinaryTexture>(std::move(*(_matex->texture2d)));
        }
        MaterialTexture(const OrdinaryTexture::Ptr &_texture2d)
        {
            texture2d = _texture2d;
            factor = glm::vec3(-1.f);
        }
        MaterialTexture(OrdinaryTexture::Ptr &&_texture2d)
        {
            texture2d = std::move(_texture2d);
            factor = glm::vec3(-1.f);
        }
        MaterialTexture &operator=(OrdinaryTexture::Ptr _texture2d)
        {
            if (this != nullptr)
                texture2d = _texture2d;
            return *this;
        }
        MaterialTexture &operator=(glm::vec3 val)
        {
            if (this != nullptr)
                factor = val;
            return *this;
        }
        MaterialTexture &operator=(float val)
        {
            if (this != nullptr)
                factor.r = val;
            return *this;
        }

        nlohmann::json ToMaterialFile()
        {
            nlohmann::json j;
            j["factor"] = factor;
            if (texture2d != nullptr)
            {
                auto name = texture2d->GetPath();
                auto start = name.find_last_of("/\\");
                j["path"] = "@CURRENTDIR";
                j["texture"] = name.substr(start + 1, name.size() - start - 1);
                j["type"] = texture2d->GetType();
            }
            else
                j["texture"] = "null";
            return j;
        }
        inline static MaterialTexture::Ptr CreateFromMaterialFile(nlohmann::json j)
        {
            auto _ptr = std::make_shared<MaterialTexture>();
            _ptr->factor = j["factor"];
            if (j["texture"] != "null")
            {
                std::string path_name = std::string(j["path"]) + '/' + std::string(j["texture"]);
                _ptr->texture2d = std::make_shared<OrdinaryTexture>(path_name, static_cast<Texture::Type>(j["type"]));
            }
            return _ptr;
        }
    };

    struct Material
    {
        using Ptr = std::shared_ptr<Material>;
        enum class Type
        {
            None,
            PBR
        };

        Type materialType;
        MaterialTexture::Ptr normal{nullptr}, height{nullptr};

        Material(Type _materialType) : materialType(_materialType) {}
        Material(const Material &material) : materialType(material.materialType), normal(material.normal), height(height){}
        Material(Material &&material) noexcept : materialType(std::move(material).materialType), normal(std::move(material).normal), height(std::move(material).height) {}
        Material() : materialType(Type::None){};
        virtual ~Material() = default;
        virtual nlohmann::json ToMaterialFile()
        {
            nlohmann::json j;
            j["type"] = static_cast<int>(materialType);
            j["height"] = (height != nullptr) ? height->ToMaterialFile() : "null";
            j["normal"] = (normal != nullptr) ? normal->ToMaterialFile() : "null";
            return j;
        }
    };
}