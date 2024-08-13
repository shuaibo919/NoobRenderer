#pragma once

#include "Graphics/RHI/Texture.h"

#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKTexture2D : public Texture2D
        {
            friend class VKContext;
            friend class VKSwapChain;
            friend class VKPipeline;

        public:
            using Ptr = SharedPtr<VKTexture2D>;
            VKTexture2D(RenderContext *ctx, Properties *&&pProperties);
            VKTexture2D(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~VKTexture2D();

        protected:
            /// @brief This protected part is intended for use with swapchain to create a swapchain image.
            ///        The Texture2D::Properties may not be complete in this context,
            ///        with only width, height being considered valid properties.
            ///        Todo: A better solution.
            VKTexture2D(RenderContext *ctx, VkImage img, VkImageView view, VkFormat format, Properties *&&pProperties);

        public:
            void *GetHandle() const override;
            void *GetDescriptorInfo() override { return &mDescriptor; }
            void Bind(uint32_t slot = 0) const override {};
            void Unbind(uint32_t slot = 0) const override {};

        public:
            VkImageView GetMipImageView(uint32_t mip);
            VkImageView GetImageView() const { return mTextureImageView; }
            VkFormat GetFormat() const { return mVKFormat; }
            VkImageLayout GetImageLayout() const { return mImageLayout; }
            VkImage GetImage() const { return mTextureImage; }
            VkSampler GetSampler() const { return mTextureSampler; }

            void TransitionImage(VkImageLayout newLayout, VkCommandBuffer vkCmdHandle);

        private:
            void Destroy();
            void UpdateDescriptor();
            void PrepareTexture();
            void PrepareTexture(const std::string &path);

        private:
            uint32_t mMipLevels;
            VkFormat mVKFormat;
            VkImage mTextureImage;
            VkSampler mTextureSampler;
            VkImageLayout mImageLayout;
            VmaAllocation mAllocation{};
            VkImageView mTextureImageView;
            VkDescriptorImageInfo mDescriptor;
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
            /* TODO */
            VkImageView GetImageView(uint32_t layer) const { return VK_NULL_HANDLE; }

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

            /* TODO */
            VkImageView GetImageView(uint32_t layer) const { return VK_NULL_HANDLE; }
            VkImageView GetImageView(uint32_t layer, uint32_t mip) const { return VK_NULL_HANDLE; }

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
            /* TODO */
            VkImageView GetImageView(uint32_t layer) const { return VK_NULL_HANDLE; }

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };
    }
}
