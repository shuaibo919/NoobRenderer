#ifndef PLUTO_VK_H
#define PLUTO_VK_H

#if defined(_WIN32) && defined(WIN32)

#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#elif defined(__APPLE__)

#ifndef VK_USE_PLATFORM_MACOS_MVK
#define VK_USE_PLATFORM_MACOS_MVK
#endif

#elif defined(__linux__)
// Todo

#endif

#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

#endif