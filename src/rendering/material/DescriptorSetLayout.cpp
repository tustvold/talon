#include "DescriptorSetLayout.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/singleton/DeviceManager.hpp"

USING_TALON_NS;

vk::DescriptorSetLayout DescriptorSetLayoutBase::create(vk::DescriptorSetLayoutCreateInfo &createInfo) {
    return RenderServiceTable::deviceManager->createDescriptorSetLayout(createInfo);
}
void DescriptorSetLayoutBase::destroy(vk::DescriptorSetLayout layout) {
    RenderServiceTable::deviceManager->destroyDescriptorSetLayout(layout);
}
