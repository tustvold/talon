#include "DescriptorSet.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/singleton/DeviceManager.hpp"

USING_TALON_NS;

vk::DescriptorSetLayout DescriptorSetBase::create(vk::DescriptorSetLayoutCreateInfo &createInfo) {
    return RenderServiceTable::deviceManager->createDescriptorSetLayout(createInfo);
}
void DescriptorSetBase::destroy(vk::DescriptorSetLayout layout) {
    RenderServiceTable::deviceManager->destroyDescriptorSetLayout(layout);
}
