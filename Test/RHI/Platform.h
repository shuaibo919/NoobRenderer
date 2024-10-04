#pragma once
#if defined(VULKAN_BACKEND)
#define PLATFORM_API RenderAPI::VULKAN
#elif defined(OPENGL_BACKEND)
#define PLATFORM_API RenderAPI::OPENGL
#else
#define PLATFORM_API RenderAPI::None
#error "No backend defined"
#endif