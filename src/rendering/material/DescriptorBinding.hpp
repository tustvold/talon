#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

template <typename Component, vk::DescriptorType type = Component::getDescriptorType()>
struct DescriptorBinding;

template <typename Component>
struct DescriptorBinding<Component, vk::DescriptorType::eUniformBuffer> {
    explicit DescriptorBinding(int binding) : binding(binding) {

    }
    DescriptorBinding(const DescriptorBinding&) = delete;
    DescriptorBinding(DescriptorBinding&&) = delete;

    const int binding;
};


template <typename Component>
struct DescriptorBinding<Component, vk::DescriptorType::eSampler> {
    explicit DescriptorBinding(int binding) : binding(binding) {

    }
    DescriptorBinding(const DescriptorBinding&) = delete;
    DescriptorBinding(DescriptorBinding&&) = delete;

    const int binding;
};

TALON_NS_END