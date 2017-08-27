#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <rendering/singleton/MemoryAllocator.hpp>

TALON_NS_BEGIN

class MockMemoryAllocator : public MemoryAllocator {
public:
    MOCK_CONST_METHOD5(createBuffer, vk::Result(const VkBufferCreateInfo *pCreateInfo,
                            const VmaMemoryRequirements *pMemoryRequirements,
                            VkBuffer *pBuffer,
                            VmaAllocation *pAllocation,
                            VmaAllocationInfo *pAllocationInfo));
    MOCK_CONST_METHOD2(destroyBuffer, void(vk::Buffer buffer, VmaAllocation allocation));
    MOCK_CONST_METHOD2(mapMemory, vk::Result(VmaAllocation allocation, void **ptr));
    MOCK_CONST_METHOD1(unmapMemory, void(VmaAllocation allocation));
};

TALON_NS_END