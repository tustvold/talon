#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class DescriptorManager {
public:
    virtual vk::DescriptorSet allocateDescriptorSet(const vk::DescriptorSetLayout& layout) = 0;

    virtual void freeDescriptorSet(vk::DescriptorSet) = 0;
};

TALON_NS_END