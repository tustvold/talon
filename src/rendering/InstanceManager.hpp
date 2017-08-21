#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

struct ApplicationInitSettings;

class InstanceManager {
public:
    explicit InstanceManager(const ApplicationInitSettings& initSettings);
    ~InstanceManager();

    vk::Instance getInstance() const {
        return instance;
    }

private:
    vk::Instance instance;
};

TALON_NS_END


