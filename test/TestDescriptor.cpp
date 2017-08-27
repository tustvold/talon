#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ecs/annotations/AnnotationDependency.hpp>
#include <ecs/TWorld.hpp>
#include <vulkan/vulkan.hpp>
#include <rendering/material/DescriptorSet.hpp>
#include "ecs/Components.hpp"
#include "mock/MockApplication.hpp"
#include "mock/MockDeviceManager.hpp"

USING_TALON_NS;

using namespace testing;

struct Binding1 {
    static vk::DescriptorSetLayoutBinding getDescriptorBinding() {
        const int binding = 0;
        const vk::DescriptorType descriptorType = vk::DescriptorType::eUniformBuffer;
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

TEST(TestDescriptor, Main) {
    MockApplication application;
    auto deviceManager = application.getDeviceManager();

    EXPECT_CALL(*deviceManager, createDescriptorSetLayout(testing::_)).WillOnce(Invoke([](const vk::DescriptorSetLayoutCreateInfo& in){
        EXPECT_EQ(in.bindingCount, 1);
        EXPECT_EQ(in.pBindings[0], Binding1::getDescriptorBinding());
        return vk::DescriptorSetLayout();
    }));

    DescriptorSet<Binding1>::createDescriptorSet();
}
