#include "DescriptorSet.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/singleton/DeviceManager.hpp"

USING_TALON_NS;

vk::DescriptorSetLayout detail::DescriptorSetImpl::createDescriptorSetInternal(vk::DescriptorSetLayoutCreateInfo& createInfo) {
    return RenderServiceTable::deviceManager->createDescriptorSetLayout(createInfo);
}
