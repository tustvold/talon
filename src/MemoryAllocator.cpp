#include "MemoryAllocator.hpp"

USING_TALON_NS;

MemoryAllocator::MemoryAllocator(DeviceManager *deviceManager) {
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = deviceManager->getPhysicalDevice();
    allocatorInfo.device = deviceManager->getDevice();

    vmaCreateAllocator(&allocatorInfo, &allocator);
}

MemoryAllocator::~MemoryAllocator() {
    vmaDestroyAllocator(allocator);
}
