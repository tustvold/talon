#include "CommandBuffer.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/singleton/impl/VulkanCommandPool.hpp"

talon::CommandBuffer::CommandBuffer(vk::CommandBufferLevel level) {
    commandBuffer = RenderServiceTable::commandPool->createCommandBuffers(1, level)[0];
}
talon::CommandBuffer::~CommandBuffer() {
    RenderServiceTable::commandPool->destroyCommandBuffer(commandBuffer);
}
