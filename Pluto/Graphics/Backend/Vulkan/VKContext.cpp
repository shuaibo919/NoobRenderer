/* Vulkan Context */
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKShader.h"
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKPipeline.h"
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
#include "Graphics/Backend/Vulkan/VKIndexBuffer.h"
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
#include "Graphics/Backend/Vulkan/VKVertexBuffer.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKUniformBuffer.h"
#include "Graphics/Backend/Vulkan/VKDescriptorSet.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
/* Common */

using namespace pluto;
using namespace pluto::Graphics;

const bool EnableValidationLayers = false;
#define VK_LAYER_LUNARG_STANDARD_VALIDATION_NAME "VK_LAYER_LUNARG_standard_validation"
#define VK_LAYER_LUNARG_ASSISTENT_LAYER_NAME "VK_LAYER_LUNARG_assistant_layer"
#define VK_LAYER_LUNARG_VALIDATION_NAME "VK_LAYER_KHRONOS_validation"

namespace pluto::Graphics::Vulkan
{
    Shader::Ptr CreateShader(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKShader>(ctx, std::move((Shader::Properties *)pPropeties));
    }

    VertexBuffer::Ptr CreateVertexBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKVertexBuffer>(ctx, std::move((VertexBuffer::Properties *)pPropeties));
    }

    IndexBuffer::Ptr CreateIndexBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKIndexBuffer>(ctx, std::move((IndexBuffer::Properties *)pPropeties));
    }

    RenderPass::Ptr CreateRenderPass(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKRenderPass>(ctx, std::move((RenderPass::Properties *)pPropeties));
    }

    Framebuffer::Ptr CreateFrameBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKFramebuffer>(ctx, std::move((Framebuffer::Properties *)pPropeties));
    }

    CommandBuffer::Ptr CreateCommandBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKCommandBuffer>(ctx, std::move((CommandBuffer::Properties *)pPropeties));
    }

    UniformBuffer::Ptr CreateUniformBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKUniformBuffer>(ctx, std::move((UniformBuffer::Properties *)pPropeties));
    }

    SwapChain::Ptr CreateSwapChain(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKSwapChain>(ctx, std::move((SwapChain::Properties *)pPropeties));
    }

    Pipeline::Ptr CreatePipeline(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKPipeline>(ctx, std::move((Pipeline::Properties *)pPropeties));
    }

    DescriptorSet::Ptr CreateDescriptorSet(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKDescriptorSet>(ctx, std::move((DescriptorSet::Properties *)pPropeties));
    }

    Texture::Ptr CreateTexture(uint16_t type, RenderContext *ctx, void *&&pPropeties)
    {
        Texture::Type kind = static_cast<Texture::Type>(type);
        switch (kind)
        {
        case Texture::Type::Texture2D:
            return std::make_shared<VKTexture2D>(ctx, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture2DArray:
            return std::make_shared<VKTexture2DArray>(ctx, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::TextureCube:
            return std::make_shared<VKTextureCube>(ctx, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture3D:
            return std::make_shared<VKTexture3D>(ctx, std::move((Texture::Properties *)pPropeties));
            break;

        default:
            break;
        }
        return nullptr;
    }

    Texture::Ptr CreateTexture(uint16_t type, const std::string &path, RenderContext *ctx, void *&&pPropeties)
    {
        Texture::Type kind = static_cast<Texture::Type>(type);
        switch (kind)
        {
        case Texture::Type::Texture2D:
            return std::make_shared<VKTexture2D>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture2DArray:
            return std::make_shared<VKTexture2DArray>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::TextureCube:
            return std::make_shared<VKTextureCube>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture3D:
            return std::make_shared<VKTexture3D>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;

        default:
            break;
        }
        return nullptr;
    }

    const std::vector<const char *> GetRequiredExtensions(bool enableValidationLayers)
    {
        std::vector<const char *> extensions;

        if (enableValidationLayers)
        {
            log<Info>("Vulkan Call GetRequiredExtensions: Enabling Validation Layers");
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }

        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

        extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

#if defined(_WIN32)
        extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
        extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_DIRECT2DISPLAY)
        extensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
        extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
        extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
        extensions.push_back("VK_EXT_metal_surface");
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
        extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_METAL_EXT)
        extensions.push_back("VK_EXT_metal_surface");
#endif

        return extensions;
    }

    const std::vector<const char *> GetRequiredLayers(bool enableValidationLayers)
    {
        std::vector<const char *> layers;

        if (enableValidationLayers)
        {
            layers.emplace_back(VK_LAYER_LUNARG_VALIDATION_NAME);
        }

        return layers;
    }

    VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback)
    {
        auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));

        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pCallback);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator)
    {
        auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(
            instance, "vkDestroyDebugReportCallbackEXT"));

        if (func != nullptr)
        {
            func(instance, callback, pAllocator);
        }
    }

    VkBool32 DebugCallback(VkDebugReportFlagsEXT flags,
                           VkDebugReportObjectTypeEXT objType,
                           uint64_t sourceObj,
                           size_t location,
                           int32_t msgCode,
                           const char *pLayerPrefix,
                           const char *pMsg,
                           void *userData)
    {
        if (!flags)
            return VK_FALSE;

        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        {
            log<Error>("[VULKAN] - ERROR : [%s] Code {%d}  : {%s}", pLayerPrefix, msgCode, pMsg);
        }
        if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
        {
            log<Warn>("[VULKAN] - WARNING : [%s] Code {%d}  : {%s}", pLayerPrefix, msgCode, pMsg);
        }
        if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        {
            log<Warn>("[VULKAN] - PERFORMANCE : [%s] Code {%d}  : {%s}", pLayerPrefix, msgCode, pMsg);
        }
        if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
        {
            log<Warn>("[VULKAN] - INFO : [%s] Code {%d}  : {%s}", pLayerPrefix, msgCode, pMsg);
        }
        if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
        {
            log<Debug>("[VULKAN] - DEBUG : [%s] Code {%d}  : {%s}", pLayerPrefix, msgCode, pMsg);
        }

        return VK_FALSE;
    }
}

VKContext::VKContext()
{
    mRenderContext = new VKRenderContext(this);
    mRenderAPI = RenderAPI::VULKAN;
}

VKContext::~VKContext()
{
    if (mDebugCallback != nullptr)
        Vulkan::DestroyDebugReportCallbackEXT(mVkInstance, mDebugCallback, VK_NULL_HANDLE);

    if (mRenderDevice != nullptr)
        RenderDevice::Release(mRenderDevice);

    vkDestroyInstance(mVkInstance, nullptr);
}

void VKContext::Present()
{
}

void VKContext::OnImGui()
{
}

void VKContext::BindToDevice()
{
    mRenderDevice = static_cast<VKRenderDevice *>(VKRenderDevice::Create(this->Get()));
}

void VKContext::Init()
{
    bool enableValidation = EnableValidationLayers;

    // {
    //     enableValidation = true;
    // }

    mInstanceLayerNames = Vulkan::GetRequiredLayers(enableValidation);
    mInstanceExtensionNames = Vulkan::GetRequiredExtensions(enableValidation);

    if (!CheckValidationLayerSupport(mInstanceLayerNames))
    {
        log<Info>("VULKAN: Validation layers requested are not available!");
    }

    if (!CheckExtensionSupport(mInstanceExtensionNames))
    {
        log<Info>("VULKAN: Validation layers requested are not available!");
    }
    VkApplicationInfo appInfo = {};

    uint32_t sdkVersion = VK_HEADER_VERSION_COMPLETE;
    uint32_t driverVersion = 0;

    auto enumerateInstanceVersion = reinterpret_cast<PFN_vkEnumerateInstanceVersion>(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));

    if (enumerateInstanceVersion)
    {
        enumerateInstanceVersion(&driverVersion);
    }
    else
    {
        driverVersion = VK_API_VERSION_1_0;
    }

    // Choose supported version
    appInfo.apiVersion = sdkVersion < driverVersion ? sdkVersion : driverVersion;

    mVKVersion = appInfo.apiVersion;

    // SDK not supported
    if (sdkVersion > driverVersion)
    {
        auto sdkVersionStr = std::to_string(VK_API_VERSION_MAJOR(sdkVersion)) + "." + std::to_string(VK_API_VERSION_MINOR(sdkVersion)) + "." + std::to_string(VK_API_VERSION_PATCH(sdkVersion));
        auto driverVersionStr = std::to_string(VK_API_VERSION_MAJOR(driverVersion)) + "." + std::to_string(VK_API_VERSION_MINOR(driverVersion)) + "." + std::to_string(VK_API_VERSION_PATCH(driverVersion));
        log<Info>("The driver SDK may need to update. SDK: %s Driver: %s", sdkVersionStr.c_str(), driverVersionStr.c_str());
    }

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Runtime";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "NoobRenderer-Pluto";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo createInfo = {};
    createInfo.pApplicationInfo = &appInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(mInstanceExtensionNames.size());
    createInfo.ppEnabledExtensionNames = mInstanceExtensionNames.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(mInstanceLayerNames.size());
    createInfo.ppEnabledLayerNames = mInstanceLayerNames.data();
    createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    const std::vector<const char *> validation_layers = {"VK_LAYER_KHRONOS_validation"};

    const bool enableFeatureValidation = false;

    if (enableFeatureValidation)
    {
        std::vector<VkValidationFeatureEnableEXT> validation_extensions = {};
        validation_extensions.emplace_back(VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT);
        validation_extensions.emplace_back(VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT);

        VkValidationFeaturesEXT validation_features = {};
        validation_features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
        validation_features.enabledValidationFeatureCount = static_cast<uint32_t>(validation_extensions.size());
        validation_features.pEnabledValidationFeatures = validation_extensions.data();
        createInfo.pNext = &validation_features;
    }

    VkResult createResult = vkCreateInstance(&createInfo, nullptr, &mVkInstance);
    if (createResult != VK_SUCCESS)
    {
        log<Info>("Failed to create Vulkan instance! Error code: {0}", createResult);
    }
    log<Info>("Vulkan instance created!");

    if (enableValidation)
    {
        SetupDebugMessenger();
    }
    mRenderContext->Init();
}

bool VKContext::CheckValidationLayerSupport(std::vector<const char *> &validationLayers)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    mInstanceLayers.resize(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, mInstanceLayers.data());
    bool removedLayer = false;

    validationLayers.erase(
        std::remove_if(
            validationLayers.begin(),
            validationLayers.end(),
            [&](const char *layerName)
            {
                bool layerFound = false;

                for (auto &layerProperties : mInstanceLayers)
                {
                    if (strcmp(layerName, layerProperties.layerName) == 0)
                    {
                        layerFound = true;
                        break;
                    }
                }

                if (!layerFound)
                {
                    removedLayer = true;
                    log<Info>("Layer not supported - {0}", layerName);
                }

                return !layerFound;
            }),
        validationLayers.end());

    return !removedLayer;
}

bool VKContext::CheckExtensionSupport(std::vector<const char *> &extensions)
{
    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    mInstanceExtensions.resize(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, mInstanceExtensions.data());

    bool removedExtension = false;

    extensions.erase(
        std::remove_if(
            extensions.begin(),
            extensions.end(),
            [&](const char *extensionName)
            {
                bool extensionFound = false;

                for (const auto &extensionProperties : mInstanceExtensions)
                {
                    if (strcmp(extensionName, extensionProperties.extensionName) == 0)
                    {
                        extensionFound = true;
                        break;
                    }
                }

                if (!extensionFound)
                {
                    removedExtension = true;
                    log<Info>("Layer not supported - {0}", extensionName);
                }

                return !extensionFound;
            }),
        extensions.end());

    return !removedExtension;
}

void VKContext::SetupDebugMessenger()
{

    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    createInfo.pfnCallback = reinterpret_cast<PFN_vkDebugReportCallbackEXT>(Vulkan::DebugCallback);

    VkResult result = Vulkan::CreateDebugReportCallbackEXT(mVkInstance, &createInfo, nullptr, &mDebugCallback);
    if (result != VK_SUCCESS)
    {
        log<Info>("Failed to set up debug callback!");
    }
}

SharedPtr<GraphicsContext> VKContext::Create()
{
    return std::make_shared<VKContext>();
}

Shader::Ptr VKContext::CreateShader(void *&&pPropeties)
{
    return Vulkan::CreateShader(mRenderContext, std::forward<void *>(pPropeties));
}

VertexBuffer::Ptr VKContext::CreateVertexBuffer(void *&&pPropeties)
{
    return Vulkan::CreateVertexBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

IndexBuffer::Ptr VKContext::CreateIndexBuffer(void *&&pPropeties)
{
    return Vulkan::CreateIndexBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

RenderPass::Ptr VKContext::CreateRenderPass(void *&&pPropeties)
{
    return Vulkan::CreateRenderPass(mRenderContext, std::forward<void *>(pPropeties));
}

Framebuffer::Ptr VKContext::CreateFrameBuffer(void *&&pPropeties)
{
    return Vulkan::CreateFrameBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<CommandBuffer> VKContext::CreateCommandBuffer(void *&&pPropeties)
{
    return Vulkan::CreateCommandBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<SwapChain> VKContext::CreateSwapChain(void *&&pPropeties)
{
    return Vulkan::CreateSwapChain(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<UniformBuffer> VKContext::CreateUniformBuffer(void *&&pPropeties)
{
    return Vulkan::CreateUniformBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<Pipeline> VKContext::CreatePipeline(void *&&pPropeties)
{
    return Vulkan::CreatePipeline(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<DescriptorSet> VKContext::CreateDescriptorSet(void *&&pPropeties)
{
    return Vulkan::CreateDescriptorSet(mRenderContext, std::forward<void *>(pPropeties));
}

Texture::Ptr VKContext::CreateTexture(uint16_t type, void *&&pPropeties)
{
    return Vulkan::CreateTexture(type, mRenderContext, std::forward<void *>(pPropeties));
}

Texture::Ptr VKContext::CreateTexture(uint16_t type, const std::string &path, void *&&pPropeties)
{
    return Vulkan::CreateTexture(type, path, mRenderContext, std::forward<void *>(pPropeties));
}
