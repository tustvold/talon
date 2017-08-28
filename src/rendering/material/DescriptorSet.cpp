#include "DescriptorSet.hpp"
#include <rendering/singleton/RenderServiceTable.hpp>
#include <rendering/singleton/DescriptorManager.hpp>
#include <DescriptorConfig.hpp>

USING_TALON_NS;

DescriptorSetBase::DescriptorSetBase(const vk::DescriptorSetLayout &layout) {
    descriptorSet = RenderServiceTable::descriptorManager->allocateDescriptorSet(layout);
}

DescriptorSetBase::~DescriptorSetBase() {
    RenderServiceTable::descriptorManager->freeDescriptorSet(descriptorSet);
}
