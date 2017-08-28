#include "TDescriptorPool.hpp"
#include <vulkan/vulkan.hpp>
#include <rendering/singleton/RenderServiceTable.hpp>
#include <rendering/singleton/DeviceManager.hpp>

USING_TALON_NS;
DescriptorPoolBase::DescriptorPoolBase(size_t maxSets,
                                       const DescriptorPoolBase::DescriptorPoolCountBindingsArray &array) {
    std::vector<vk::DescriptorPoolSize> poolSizes;

    for (int i = 0; i < VK_DESCRIPTOR_TYPE_END_RANGE - VK_DESCRIPTOR_TYPE_BEGIN_RANGE + 1; i++) {
        if (array[i] > 0) {
            poolSizes.emplace_back(static_cast<vk::DescriptorType>(i + VK_DESCRIPTOR_TYPE_BEGIN_RANGE),
                                   (uint32_t) array[i]);
        }
    }

    vk::DescriptorPoolCreateInfo createInfo = {};
    createInfo.maxSets = static_cast<uint32_t>(maxSets);
    createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    createInfo.pPoolSizes = poolSizes.data();
    createInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;

    descriptorPool = RenderServiceTable::deviceManager->createDescriptorPool(createInfo);

    RenderServiceTable::descriptorManager.set(this);
}

DescriptorPoolBase::~DescriptorPoolBase() {
    RenderServiceTable::descriptorManager.clear(this);
    RenderServiceTable::deviceManager->destroyDescriptorPool(descriptorPool);
}

vk::DescriptorSet DescriptorPoolBase::allocateDescriptorSet(const vk::DescriptorSetLayout &layout) {
    vk::DescriptorSetAllocateInfo info = {};
    info.descriptorPool = descriptorPool;
    info.descriptorSetCount = 1;
    info.pSetLayouts = &layout;

    vk::DescriptorSet ret;
    RenderServiceTable::deviceManager->allocateDescriptorSets(info, &ret);
    return ret;
}

void DescriptorPoolBase::freeDescriptorSet(vk::DescriptorSet set) {
    RenderServiceTable::deviceManager->freeDescriptorSets(descriptorPool, set);
}
