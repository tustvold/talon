#pragma once
#include <vulkan/vulkan.hpp>
#include "TalonConfig.hpp"
#include "Delegate.hpp"

TALON_NS_BEGIN

struct VDebugCallbackArgs {
    VkDebugReportFlagsEXT flags;
    VkDebugReportObjectTypeEXT objectType;
    uint64_t object;
    size_t location;
    int32_t messageCode;
    const char *pLayerPrefix;
    const char *pMessage;

    VDebugCallbackArgs(VkDebugReportFlagsEXT flags,
                       VkDebugReportObjectTypeEXT objectType,
                       uint64_t object,
                       size_t location,
                       int32_t messageCode,
                       const char *pLayerPrefix,
                       const char *pMessage);
};

class DebugCallback {
public:
    explicit DebugCallback(const Gallant::Delegate1<const VDebugCallbackArgs&> &delegate);
    ~DebugCallback();

private:
    Gallant::Delegate1<const VDebugCallbackArgs&> delegate;
    vk::DebugReportCallbackEXT callback;

    static VkBool32 vulkanDebugCallback(VkDebugReportFlagsEXT flags,
                                 VkDebugReportObjectTypeEXT objectType,
                                 uint64_t object,
                                 size_t location,
                                 int32_t messageCode,
                                 const char *pLayerPrefix,
                                 const char *pMessage,
                                 void *pUserData);

};

TALON_NS_END