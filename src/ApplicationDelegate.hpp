#pragma once
#include "TalonConfig.hpp"
#include "rendering/DebugCallback.hpp"
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class ApplicationDelegate {
public:
    virtual void vulkanDebugCallback(const VDebugCallbackArgs&) = 0;
};

TALON_NS_END