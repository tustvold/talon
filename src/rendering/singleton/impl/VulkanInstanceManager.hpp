#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>
#include <rendering/singleton/InstanceManager.hpp>

TALON_NS_BEGIN

struct ApplicationInitSettings;

class VulkanInstanceManager : public InstanceManager {
public:
    explicit VulkanInstanceManager(const ApplicationInitSettings& initSettings);
    ~VulkanInstanceManager() final;

    vk::Instance getInstance() const {
        return instance;
    }

    vk::DebugReportCallbackEXT createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& createInfoEXT) final {
        return instance.createDebugReportCallbackEXT(createInfoEXT);
    }

    void destroyDebugReportCallbackEXT(vk::DebugReportCallbackEXT callbackEXT) final {
        instance.destroyDebugReportCallbackEXT(callbackEXT);
    }

private:
    vk::Instance instance;
};

TALON_NS_END



