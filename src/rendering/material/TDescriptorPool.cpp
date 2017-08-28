#include "TDescriptorPool.hpp"
#include <vulkan/vulkan.hpp>
#include <rendering/singleton/RenderServiceTable.hpp>
#include <rendering/singleton/DeviceManager.hpp>

USING_TALON_NS;

vk::DescriptorPool DescriptorPoolBase::create(size_t maxSets, const DescriptorPoolCountBindingsArray& array) {


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

    return RenderServiceTable::deviceManager->createDescriptorPool(createInfo);
}

void DescriptorPoolBase::destroy(vk::DescriptorPool pool) {
    RenderServiceTable::deviceManager->destroyDescriptorPool(pool);
}
