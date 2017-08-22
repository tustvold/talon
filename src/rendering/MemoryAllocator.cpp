#include "MemoryAllocator.hpp"
#include "ApplicationServiceTable.hpp"
#include "DeviceManager.hpp"

USING_TALON_NS;

MemoryAllocator::MemoryAllocator(DeviceManager *deviceManager) {
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = deviceManager->getPhysicalDevice();
    allocatorInfo.device = deviceManager->getDevice();

    vmaCreateAllocator(&allocatorInfo, &allocator);

    ApplicationServiceTable::memoryAllocator.set(this);
}

MemoryAllocator::~MemoryAllocator() {
    ApplicationServiceTable::memoryAllocator.clear(this);
    vmaDestroyAllocator(allocator);
}
