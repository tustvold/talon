#include "MemoryAllocator.hpp"
#include "RenderServiceTable.hpp"
#include "DeviceManager.hpp"

USING_TALON_NS;

MemoryAllocator::MemoryAllocator(DeviceManager *deviceManager) {
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = deviceManager->getPhysicalDevice();
    allocatorInfo.device = deviceManager->getDevice();

    vmaCreateAllocator(&allocatorInfo, &allocator);

    RenderServiceTable::memoryAllocator.set(this);
}

MemoryAllocator::~MemoryAllocator() {
    RenderServiceTable::memoryAllocator.clear(this);
    vmaDestroyAllocator(allocator);
}
