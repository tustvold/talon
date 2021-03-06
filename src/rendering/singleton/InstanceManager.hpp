#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class InstanceManager {
public:
    virtual ~InstanceManager() = default;

    virtual vk::DebugReportCallbackEXT createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& createInfoEXT) const = 0;

    virtual void destroyDebugReportCallbackEXT(vk::DebugReportCallbackEXT callbackEXT) const = 0;
};

TALON_NS_END