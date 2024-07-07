#ifndef PLUTO_ENGINE_ASSET_H
#define PLUTO_ENGINE_ASSET_H
#include "Core/Base.hpp"

namespace pluto
{
    enum class AssetFlag
    {
        None,
        Missing,
        Invalid,
        Loaded,
        UnLoaded
    };

    enum class AssetType
    {
        Unkown,
        Texture,
        Mesh,
        Scene,
        Shader,
        Material,
        Model,
        Skeleton,
        Animation,
        Script
    };

    inline constexpr const char *AssetTypeToString(AssetType assetType)
    {
        switch (assetType)
        {
        case AssetType::Unkown:
            return "Unkown";
        case AssetType::Scene:
            return "Scene";
        case AssetType::Mesh:
            return "Mesh";
        case AssetType::Model:
            return "Model";
        case AssetType::Material:
            return "Material";
        case AssetType::Texture:
            return "Texture";
        case AssetType::Script:
            return "Script";
        case AssetType::Skeleton:
            return "Skeleton";
        case AssetType::Animation:
            return "Animation";
        case AssetType::Shader:
            return "Shader";
        }

        return "None";
    }

    class Asset
    {
    public:
        AssetFlag Flags{AssetFlag::None};

        virtual ~Asset() {}
        virtual AssetType GetAssetType() const { return AssetType::Unkown; }

        bool IsValid() const { return !((Flags == AssetFlag::Missing) || (Flags == AssetFlag::Invalid)); }

        virtual bool operator==(const Asset &other) const
        {
            return Handle == other.Handle;
        }

        virtual bool operator!=(const Asset &other) const
        {
            return !(*this == other);
        }

        bool IsFlagSet(AssetFlag flag = AssetFlag::None) const { return Flags == flag; }
        void SetFlag(AssetFlag flag, bool value = true) { Flags = flag; }

        Identity Handle;
    };

}

#endif