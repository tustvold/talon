#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ecs/annotations/AnnotationDependency.hpp>
#include <ecs/TWorld.hpp>
#include <vulkan/vulkan.hpp>
#include <rendering/material/DescriptorSet.hpp>
#include "ecs/Components.hpp"
#include "mock/MockApplication.hpp"
#include "mock/MockDeviceManager.hpp"
#include "rendering/material/TDescriptorPool.hpp"

USING_TALON_NS;

using namespace testing;

struct Binding1 {
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

struct Binding2 {
    static constexpr vk::DescriptorType getDescriptorType() {
        return vk::DescriptorType::eSampler;
    }

    static vk::DescriptorSetLayoutBinding getDescriptorBinding() {
        const int binding = 0;
        const vk::DescriptorType descriptorType = getDescriptorType();
        const uint32_t descriptorCount = 1;
        const vk::ShaderStageFlagBits shaderStageFlags = vk::ShaderStageFlagBits::eFragment;
        const vk::Sampler *immutableSamplers = nullptr;

        return vk::DescriptorSetLayoutBinding(binding,
                                              descriptorType,
                                              descriptorCount,
                                              shaderStageFlags,
                                              immutableSamplers);
    }
};

TALON_NS_BEGIN

COMPONENT_STORAGE_DEF(Binding1, ComponentStorageFlatMap);
COMPONENT_STORAGE_DEF(Binding2, ComponentStorageFlatMap);

TALON_NS_END

static_assert(
    DescriptorPoolBase::DescriptorPoolCountBindings(boost::hana::tuple_t<Binding1>, vk::DescriptorType::eUniformBuffer)
        == 1);
static_assert(
    DescriptorPoolBase::DescriptorPoolCountBindings(boost::hana::tuple_t<Binding1>, vk::DescriptorType::eSampler) == 0);
static_assert(
    DescriptorPoolBase::DescriptorPoolCountBindings(boost::hana::tuple_t<Binding2>, vk::DescriptorType::eSampler) == 1);
static_assert(
    DescriptorPoolBase::DescriptorPoolCountBindings(boost::hana::tuple_t<Binding1, Binding2>,
                                                    vk::DescriptorType::eUniformBuffer)
        == 1);
static_assert(
    DescriptorPoolBase::DescriptorPoolCountBindings(boost::hana::tuple_t<Binding1, Binding2>,
                                                    vk::DescriptorType::eSampler) == 1);

TEST(TestDescriptor, TestSingle) {
    MockApplication application;
    auto deviceManager = application.getDeviceManager();

    EXPECT_CALL(*deviceManager,
                createDescriptorSetLayout(testing::_)).WillOnce(Invoke([](const vk::DescriptorSetLayoutCreateInfo &in) {
        EXPECT_EQ(in.bindingCount, 1);
        EXPECT_EQ(in.pBindings[0], Binding1::getDescriptorBinding());
        return vk::DescriptorSetLayout();
    }));

    EXPECT_CALL(*deviceManager, destroyDescriptorSetLayout(testing::_));

    DescriptorSet<Binding1> binding;
}

TEST(TestDescriptor, TestMultiple) {
    MockApplication application;
    auto deviceManager = application.getDeviceManager();

    EXPECT_NE(Binding1::getDescriptorBinding(), Binding2::getDescriptorBinding());

    EXPECT_CALL(*deviceManager,
                createDescriptorSetLayout(testing::_))
        .WillOnce(Invoke([](const vk::DescriptorSetLayoutCreateInfo &in) {
            EXPECT_EQ(in.bindingCount, 2);
            EXPECT_EQ(in.pBindings[0], Binding1::getDescriptorBinding());
            EXPECT_EQ(in.pBindings[1], Binding2::getDescriptorBinding());
            return vk::DescriptorSetLayout();
        }));

    EXPECT_CALL(*deviceManager, destroyDescriptorSetLayout(testing::_));

    DescriptorSet<Binding1, Binding2> binding;
}

TEST(TestDescriptor, TestDescriptorCounts) {
    constexpr auto array1 = DescriptorPoolBase::getDescriptorCounts(boost::hana::tuple_t<Binding1>);
    for (int i = VK_DESCRIPTOR_TYPE_BEGIN_RANGE; i < VK_DESCRIPTOR_TYPE_END_RANGE; i++) {
        if (i == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
            EXPECT_EQ(array1[i - VK_DESCRIPTOR_TYPE_BEGIN_RANGE], 1);
        else
            EXPECT_EQ(array1[i - VK_DESCRIPTOR_TYPE_BEGIN_RANGE], 0);
    }

    constexpr auto array2 = DescriptorPoolBase::getDescriptorCounts(boost::hana::tuple_t<Binding1, Binding2>);
    for (int i = VK_DESCRIPTOR_TYPE_BEGIN_RANGE; i < VK_DESCRIPTOR_TYPE_END_RANGE; i++) {
        if (i == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER || i == VK_DESCRIPTOR_TYPE_SAMPLER)
            EXPECT_EQ(array2[i - VK_DESCRIPTOR_TYPE_BEGIN_RANGE], 1);
        else
            EXPECT_EQ(array2[i - VK_DESCRIPTOR_TYPE_BEGIN_RANGE], 0);
    }
}

static_assert(TWorld<Binding1>::HasComponent<Binding1>());
static_assert(!TWorld<Binding1>::HasComponent<Binding2>());

TEST(TestDescriptor, TestDescriptorPool) {
    using DescriptorSet1 = DescriptorSet<Binding1>;
    using DescriptorSet2 = DescriptorSet<Binding1, Binding2>;

    using World = TWorld<Binding1, Binding2>;

    InSequence seq;

    MockApplication application;
    auto deviceManager = application.getDeviceManager();

    EXPECT_CALL(*deviceManager,
                createDescriptorSetLayout(testing::_))
        .WillOnce(Invoke([](const vk::DescriptorSetLayoutCreateInfo &in) {
            EXPECT_EQ(in.bindingCount, 1);
            EXPECT_EQ(in.pBindings[0], Binding1::getDescriptorBinding());
            return vk::DescriptorSetLayout();
        }))
        .WillOnce(Invoke([](const vk::DescriptorSetLayoutCreateInfo &in) {
            EXPECT_EQ(in.bindingCount, 2);
            EXPECT_EQ(in.pBindings[0], Binding1::getDescriptorBinding());
            EXPECT_EQ(in.pBindings[1], Binding2::getDescriptorBinding());
            return vk::DescriptorSetLayout();
        }));


    EXPECT_CALL(*deviceManager, createDescriptorPool(testing::_)).WillOnce(Invoke([](const vk::DescriptorPoolCreateInfo& info){
        EXPECT_EQ(info.maxSets, 2);
        EXPECT_EQ(info.poolSizeCount, 2);
        EXPECT_EQ(info.pPoolSizes[0].descriptorCount, 1);
        EXPECT_EQ(info.pPoolSizes[1].descriptorCount, 1);
        EXPECT_NE(info.pPoolSizes[0].type == Binding1::getDescriptorType() , info.pPoolSizes[0].type == Binding2::getDescriptorType());
        EXPECT_NE(info.pPoolSizes[1].type == Binding1::getDescriptorType() , info.pPoolSizes[1].type == Binding2::getDescriptorType());

        return vk::DescriptorPool();
    }));

    EXPECT_CALL(*deviceManager, destroyDescriptorPool(testing::_));

    EXPECT_CALL(*deviceManager, destroyDescriptorSetLayout(testing::_)).Times(2);

    TDescriptorPool<World, DescriptorSet1, DescriptorSet2> pool;
}