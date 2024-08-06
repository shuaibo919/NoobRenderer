#pragma once

#include "Graphics/RHI/Texture.h"

#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKTexture2D final : public Texture2D
        {
            friend class VKContext;

        public:
            using Ptr = SharedPtr<VKTexture2D>;
            VKTexture2D(RenderContext *ctx, Properties *&&pProperties);
            VKTexture2D(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~VKTexture2D();

        public:
            /// @brief If passing a view:VkImageView, the values in pProperties are invalid
            ///        and will be automatically created based on this VkImageView.
            VKTexture2D(RenderContext *ctx, VkImageView view, Properties *&&pProperties);
            void TransitionImage(VkImageLayout newLayout, VkCommandBuffer vkCmdHandle);

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;

        private:
            void LoadTextureData(const std::string &path);
            void PrepareTexture();
            VkImageView GetMipImageView(uint32_t mip);

        private:
            uint32_t mMipLevels;
            VkFormat mVKFormat;
            VkImage mTextureImage;
            VmaAllocation mAllocation;
            VkSampler mTextureSampler;
            VkImageLayout mImageLayout;
            VkImageView mTextureImageView;
            VkDescriptorImageInfo mDescriptor;
            VkDeviceMemory mTextureImageMemory;
            std::unordered_map<uint32_t, VkImageView> mMipImageViews;
        };

        class VKTexture2DArray final : public Texture2DArray
        {
            friend class VKContext;

        public:
            using Ptr = SharedPtr<VKTexture2DArray>;
            VKTexture2DArray(RenderContext *ctx, Properties *&&pProperties);
            VKTexture2DArray(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~VKTexture2DArray();

        public:
            void TransitionImage(VkImageLayout newLayout, VkCommandBuffer vkCmdHandle);

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };

        class VKTextureCube final : public TextureCube
        {
            friend class VKContext;

        public:
            using Ptr = SharedPtr<VKTextureCube>;
            VKTextureCube(RenderContext *ctx, Properties *&&pProperties);
            VKTextureCube(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~VKTextureCube();

        public:
            void TransitionImage(VkImageLayout newLayout, VkCommandBuffer vkCmdHandle);

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };

        class VKTexture3D final : public Texture3D
        {
            friend class VKContext;

        public:
            using Ptr = SharedPtr<VKTexture3D>;
            VKTexture3D(RenderContext *ctx, Properties *&&pProperties);
            VKTexture3D(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~VKTexture3D();

        public:
            void TransitionImage(VkImageLayout newLayout, VkCommandBuffer vkCmdHandle);

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };
    }
}
