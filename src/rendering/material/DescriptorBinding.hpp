#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

template <typename Component, vk::DescriptorType type = Component::getDescriptorType()>
class DescriptorBinding;

template <typename Component>
class DescriptorBinding<Component, vk::DescriptorType::eUniformBuffer> {

};


TALON_NS_END