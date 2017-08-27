#pragma once
#include <rendering/singleton/MemoryAllocator.hpp>
#include "VulkanDeviceManager.hpp"

TALON_NS_BEGIN

class DeviceManager;

class VulkanMemoryAllocator : public MemoryAllocator {
public:
    explicit VulkanMemoryAllocator(VulkanDeviceManager *deviceManager);
    ~VulkanMemoryAllocator();

    vk::Result createBuffer(const VkBufferCreateInfo *pCreateInfo,
                            const VmaMemoryRequirements *pMemoryRequirements,
                            VkBuffer *pBuffer,
                            VmaAllocation *pAllocation,
                            VmaAllocationInfo *pAllocationInfo) const {

        return static_cast<vk::Result>(vmaCreateBuffer(allocator,
                                                       pCreateInfo,
                                                       pMemoryRequirements,
                                                       pBuffer,
                                                       pAllocation,
                                                       pAllocationInfo));
    }

    void destroyBuffer(vk::Buffer buffer, VmaAllocation allocation) {
        vmaDestroyBuffer(allocator, buffer, allocation);
    }

    vk::Result mapMemory(VmaAllocation allocation, void **ptr) {
        return static_cast<vk::Result>(vmaMapMemory(allocator, allocation, ptr));
    }

    void unmapMemory(VmaAllocation allocation) {
        vmaUnmapMemory(allocator, allocation);
    }

private:
    VmaAllocator allocator;
};
TALON_NS_END