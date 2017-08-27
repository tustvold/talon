#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>
#include <rendering/singleton/MemoryAllocator.hpp>
#include "vulkan_mem_alloc.hpp"
#include "AllocatedData.hpp"

TALON_NS_BEGIN

using MappedBufferHandle = AllocatedDataMapHandle<vk::Buffer>;

struct Buffer : public AllocatedData<vk::Buffer> {
    Buffer(const vk::BufferCreateInfo *pCreateInfo,
           const VmaMemoryRequirements *pMemoryRequirements);

    ~Buffer();
};


TALON_NS_END


