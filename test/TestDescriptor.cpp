#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ecs/annotations/AnnotationDependency.hpp>
#include <ecs/TWorld.hpp>
#include <vulkan/vulkan.hpp>
#include <rendering/material/DescriptorSet.hpp>
#include "ecs/Components.hpp"

USING_TALON_NS;

struct Binding1 {
    static vk::DescriptorSetLayoutBinding getDescriptorBinding() {
        const int binding = 0;
        const vk::DescriptorType descriptorType = vk::DescriptorType::eUniformBuffer;
        const uint32_t descriptorCount = 1;
        const vk::ShaderStageFlagBits shaderStageFlags = vk::ShaderStageFlagBits::eVertex;
        const vk::Sampler* immutableSamplers = nullptr;

        return vk::DescriptorSetLayoutBinding(binding, descriptorType, descriptorCount, shaderStageFlags, immutableSamplers);
    }
};

using DescriptorSet1 = ;

TEST(TestDescriptor, Main) {

    DescriptorSet<Binding1> descriptorSet;

}
