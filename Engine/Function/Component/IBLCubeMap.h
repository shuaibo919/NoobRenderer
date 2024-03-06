#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/Serialization.h"
#include "Engine/Function/Buffer/All.h"
#include "Engine/Function/Graphics/Texture/All.h"
namespace NoobRenderer
{
    namespace component
    {
        // Beta
        struct IBLCubeMap
        {
            std::string name;
            bool valid{false};
            int cubemap_size{1024};
            int mipmap_size{128};
            int conv_size{32};
            int brdf_size{512};
            OrdinaryTexture::Ptr RawHDRTexture{nullptr};
            CubeMapTexture::Ptr HDRCubeMap{nullptr};
            CubeMapTexture::Ptr IrridianceMap{nullptr};
            CubeMapTexture::Ptr PrefilterMap{nullptr};
            Texture2D::Ptr BRDFLookupTexture{nullptr};
            IBLCubeMap() {}
            void Reset()
            {
                if (this->valid == false)
                    return;
                this->valid = false;
                RawHDRTexture.reset();
                HDRCubeMap.reset();
                IrridianceMap.reset();
                PrefilterMap.reset();
                BRDFLookupTexture.reset();
            }
            static void ConductPreCalculation(void *&typeAny);
            void PreCalculation();
            SERIALIZATION_COMPONENT_ENABLE(IBLCubeMap, name, cubemap_size, mipmap_size, conv_size, brdf_size)
            SERIALIZATION_FROM_TYPE_ANY(IBLCubeMap)
        };

    } // namespace component

};
