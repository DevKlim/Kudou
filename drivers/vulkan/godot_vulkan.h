#pragma once

#ifdef USE_VOLK
#include <volk.h>
#else
#include <cstdint>
#define VK_NO_STDINT_H
#include <vulkan/vulkan.h>
#endif
