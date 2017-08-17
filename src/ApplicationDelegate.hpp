#pragma once
#include <vulkan/vulkan.hpp>

class ApplicationDelegate {
public:
    virtual VkBool32 vulkanDebugCallback(VkDebugReportFlagsEXT flags,
                             VkDebugReportObjectTypeEXT objectType,
                             uint64_t object,
                             size_t location,
                             int32_t messageCode,
                             const char *pLayerPrefix,
                             const char *pMessage) = 0;
};