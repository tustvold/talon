#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <rendering/singleton/CommandPool.hpp>

TALON_NS_BEGIN

// Find: ([^ ]*) ([^\W]*)(\([^)]*\)) (?:const )*override \{(\n[^}]*)*}
// Replace: MOCK_CONST_METHOD($2, $1$3);
class MockCommandPool : public CommandPool {
    MOCK_CONST_METHOD1(destroyCommandBuffers, void(const std::vector<vk::CommandBuffer> &commandBuffers));
    MOCK_CONST_METHOD2(createCommandBuffers, std::vector<vk::CommandBuffer>(uint32_t count, vk::CommandBufferLevel level));
    MOCK_CONST_METHOD1(destroyCommandBuffer, void(vk::CommandBuffer buffer));
    MOCK_CONST_METHOD1(createCommandBuffer, vk::CommandBuffer(vk::CommandBufferLevel level));
};


TALON_NS_END