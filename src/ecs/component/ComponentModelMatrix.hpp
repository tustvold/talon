#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include "Eigen/Core"

TALON_NS_BEGIN

struct ComponentModelMatrix {
    Eigen::Matrix4f modelMatrix;

    static constexpr vk::DescriptorType getDescriptorType() {
        return vk::DescriptorType::eUniformBuffer;
    }

    static vk::DescriptorSetLayoutBinding getDescriptorBinding() {
        const int binding = 0;
        const vk::DescriptorType descriptorType = getDescriptorType();
        const uint32_t descriptorCount = 1;
        const vk::ShaderStageFlagBits shaderStageFlags = vk::ShaderStageFlagBits::eVertex;
        const vk::Sampler *immutableSamplers = nullptr;

        return vk::DescriptorSetLayoutBinding(binding,
                                              descriptorType,
                                              descriptorCount,
                                              shaderStageFlags,
                                              immutableSamplers);
    }
};

TALON_NS_END