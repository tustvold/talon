#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>
#include "vulkan_mem_alloc.hpp"
#include "util/Logging.hpp"

TALON_NS_BEGIN

class DeviceManager;

class MemoryAllocator {
public:
    virtual ~MemoryAllocator() = default;

    virtual vk::Result createBuffer(const VkBufferCreateInfo *pCreateInfo,
                            const VmaMemoryRequirements *pMemoryRequirements,
                            VkBuffer *pBuffer,
                            VmaAllocation *pAllocation,
                            VmaAllocationInfo *pAllocationInfo) const = 0;

    virtual void destroyBuffer(vk::Buffer buffer, VmaAllocation allocation) = 0;

    virtual vk::Result mapMemory(VmaAllocation allocation, void **ptr) = 0;

    virtual void unmapMemory(VmaAllocation allocation) = 0;
};
TALON_NS_END