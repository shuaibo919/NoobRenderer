#ifndef PLUTO_VK_H
#define PLUTO_VK_H

#if defined(_WIN32) && defined(WIN32)

#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#elif defined(__APPLE__)

#ifndef VK_USE_PLATFORM_METAL_EXT
#define VK_USE_PLATFORM_METAL_EXT
#endif

#elif defined(__linux__)
// Todo

#endif

#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

#endif