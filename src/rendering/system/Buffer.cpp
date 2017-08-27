#include <rendering/singleton/RenderServiceTable.hpp>
#include "Buffer.hpp"
USING_TALON_NS;

Buffer::Buffer(const vk::BufferCreateInfo *pCreateInfo, const VmaMemoryRequirements *pMemoryRequirements) {
    requestedSize = pCreateInfo->size;

    auto result = RenderServiceTable::memoryAllocator->createBuffer(
        reinterpret_cast<const VkBufferCreateInfo *>(pCreateInfo),
        pMemoryRequirements,
        reinterpret_cast<VkBuffer *>(&data),
        &allocation,
        &allocationInfo);
    TASSERT(result == vk::Result::eSuccess);
}

Buffer::~Buffer() {
    RenderServiceTable::memoryAllocator->destroyBuffer(data, allocation);
}

