#include <rendering/singleton/RenderServiceTable.hpp>
#include "VulkanMemoryAllocator.hpp"

USING_TALON_NS;

VulkanMemoryAllocator::VulkanMemoryAllocator(VulkanDeviceManager *deviceManager) {
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.device = deviceManager->getDevice();
    allocatorInfo.physicalDevice = deviceManager->getPhysicalDevice();
    vmaCreateAllocator(&allocatorInfo, &allocator);

    RenderServiceTable::memoryAllocator.set(this);
}

VulkanMemoryAllocator::~VulkanMemoryAllocator() {
    RenderServiceTable::memoryAllocator.clear(this);
    vmaDestroyAllocator(allocator);
}


