#include "VulkanCommandPool.hpp"
#include "rendering/singleton/impl/VulkanDeviceManager.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"

USING_TALON_NS;

VulkanCommandPool::VulkanCommandPool(VulkanDeviceManager *deviceManager, VulkanSurfaceManager *surfaceManager) {
    QueueFamilyIndices queueFamilyIndices = deviceManager->getQueueFamilyIndices(surfaceManager);

    vk::CommandPoolCreateInfo poolInfo = {};
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndices.graphicsFamily);

    commandPool = deviceManager->createCommandPool(poolInfo);

    RenderServiceTable::commandPool.set(this);
}

VulkanCommandPool::~VulkanCommandPool() {
    RenderServiceTable::commandPool.clear(this);
    RenderServiceTable::deviceManager->destroyCommandPool(commandPool);
}

std::vector<vk::CommandBuffer> VulkanCommandPool::createCommandBuffers(uint32_t count, vk::CommandBufferLevel level) const {
    vk::CommandBufferAllocateInfo allocInfo = {};
    allocInfo.commandPool = commandPool;
    allocInfo.level = level;
    allocInfo.commandBufferCount = count;
    return RenderServiceTable::deviceManager->allocateCommandBuffers(allocInfo);
}

void VulkanCommandPool::destroyCommandBuffers(const std::vector<vk::CommandBuffer> &commandBuffers) const {
    RenderServiceTable::deviceManager->freeCommandBuffers(commandPool, commandBuffers);
}
void VulkanCommandPool::destroyCommandBuffer(vk::CommandBuffer buffer) const {
    RenderServiceTable::deviceManager->freeCommandBuffers(commandPool, 1, &buffer);
}

vk::CommandBuffer VulkanCommandPool::createCommandBuffer(vk::CommandBufferLevel level) const {
    return createCommandBuffers(1, level)[0];
}
