#include "MemoryAllocator.hpp"
#include "ServiceTable.hpp"
#include "DeviceManager.hpp"

USING_TALON_NS;

MemoryAllocator::MemoryAllocator(DeviceManager *deviceManager) {
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = deviceManager->getPhysicalDevice();
    allocatorInfo.device = deviceManager->getDevice();

    vmaCreateAllocator(&allocatorInfo, &allocator);

    ServiceTable::memoryAllocator.set(this);
}

MemoryAllocator::~MemoryAllocator() {
    ServiceTable::memoryAllocator.clear(this);
    vmaDestroyAllocator(allocator);
}
