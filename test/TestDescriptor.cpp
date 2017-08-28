#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ecs/annotations/AnnotationDependency.hpp>
#include <vulkan/vulkan.hpp>
#include <rendering/material/DescriptorSetLayout.hpp>
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

struct Binding3 {
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

    DescriptorSetLayout<2, Binding1> binding;
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

    DescriptorSetLayout<2, Binding1, Binding2> binding;
}

TEST(TestDescriptor, TestDescriptorPool) {
    using DescriptorSet1 = DescriptorSetLayout<4, Binding1>;
    using DescriptorSet2 = DescriptorSetLayout<3, Binding1, Binding2>;

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
        EXPECT_EQ(info.maxSets, 7);
        EXPECT_EQ(info.poolSizeCount, 2);

        for (int i = 0; i < 2; i++) {
            auto& pool = info.pPoolSizes[0];
            if (pool.type == vk::DescriptorType::eUniformBuffer) {
                EXPECT_EQ(pool.descriptorCount, 7);
            } else if (pool.type == vk::DescriptorType::eSampler) {
                EXPECT_EQ(pool.descriptorCount, 3);
            } else {
                EXPECT_TRUE(false);
            }
        }
        return vk::DescriptorPool();
    }));

    EXPECT_CALL(*deviceManager, destroyDescriptorPool(testing::_));

    EXPECT_CALL(*deviceManager, destroyDescriptorSetLayout(testing::_)).Times(2);

    TDescriptorPool<DescriptorSet1, DescriptorSet2> pool;
}


using DescriptorSet1 = DescriptorSetLayout<4, Binding1>;
using DescriptorSet2 = DescriptorSetLayout<2, Binding1, Binding2>;
using DescriptorSet3 = DescriptorSetLayout<5, Binding2, Binding3>;

TEST(TestDescriptor, TestDescriptorPool3) {


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
        }))
        .WillOnce(Invoke([](const vk::DescriptorSetLayoutCreateInfo &in) {
            EXPECT_EQ(in.bindingCount, 2);
            EXPECT_EQ(in.pBindings[0], Binding2::getDescriptorBinding());
            EXPECT_EQ(in.pBindings[1], Binding3::getDescriptorBinding());
            return vk::DescriptorSetLayout();
        }));


    EXPECT_CALL(*deviceManager, createDescriptorPool(testing::_)).WillOnce(Invoke([](const vk::DescriptorPoolCreateInfo& info){
        EXPECT_EQ(info.maxSets, 11);
        EXPECT_EQ(info.poolSizeCount, 2);

        for (int i = 0; i < 2; i++) {
            auto& pool = info.pPoolSizes[0];
            if (pool.type == vk::DescriptorType::eUniformBuffer) {
                EXPECT_EQ(pool.descriptorCount, 6);
            } else if (pool.type == vk::DescriptorType::eSampler) {
                EXPECT_EQ(pool.descriptorCount, 12);
            } else {
                EXPECT_TRUE(false);
            }
        }
        return vk::DescriptorPool();
    }));

    EXPECT_CALL(*deviceManager, destroyDescriptorPool(testing::_));

    EXPECT_CALL(*deviceManager, destroyDescriptorSetLayout(testing::_)).Times(3);

    TDescriptorPool<DescriptorSet1, DescriptorSet2, DescriptorSet3> pool;
}


TEST(TestDescriptor, TestDescriptorPool4) {
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
        }))
        .WillOnce(Invoke([](const vk::DescriptorSetLayoutCreateInfo &in) {
            EXPECT_EQ(in.bindingCount, 2);
            EXPECT_EQ(in.pBindings[0], Binding2::getDescriptorBinding());
            EXPECT_EQ(in.pBindings[1], Binding3::getDescriptorBinding());
            return vk::DescriptorSetLayout();
        }));


    EXPECT_CALL(*deviceManager, createDescriptorPool(testing::_)).WillOnce(Invoke([](const vk::DescriptorPoolCreateInfo& info){
        EXPECT_EQ(info.maxSets, 11);
        EXPECT_EQ(info.poolSizeCount, 2);

        for (int i = 0; i < 2; i++) {
            auto& pool = info.pPoolSizes[0];
            if (pool.type == vk::DescriptorType::eUniformBuffer) {
                EXPECT_EQ(pool.descriptorCount, 6);
            } else if (pool.type == vk::DescriptorType::eSampler) {
                EXPECT_EQ(pool.descriptorCount, 12);
            } else {
                EXPECT_TRUE(false);
            }
        }
        return vk::DescriptorPool();
    }));

    EXPECT_CALL(*deviceManager, destroyDescriptorPool(testing::_));

    EXPECT_CALL(*deviceManager, destroyDescriptorSetLayout(testing::_)).Times(3);

    TDescriptorPool<DescriptorSet1, DescriptorSet2, DescriptorSet3> pool;

    EXPECT_CALL(*deviceManager, allocateDescriptorSets(testing::_, testing::_)).WillOnce(Invoke([](const vk::DescriptorSetAllocateInfo& info, vk::DescriptorSet* _){
        EXPECT_EQ(info.descriptorSetCount, 1);
        EXPECT_EQ(info.descriptorPool, vk::DescriptorPool());
    }));
    EXPECT_CALL(*deviceManager, freeDescriptorSets(testing::_, testing::_));

    auto set = pool.createDescriptorSet<DescriptorSet1>();
}