#include "CommandPool.hpp"
#include "ServiceTable.hpp"

talon::CommandPool::CommandPool(talon::DeviceManager *deviceManager, SurfaceManager* surfaceManager) {
    QueueFamilyIndices queueFamilyIndices = deviceManager->getQueueFamilyIndices(surfaceManager);

    vk::CommandPoolCreateInfo poolInfo = {};
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndices.graphicsFamily);

    commandPool = deviceManager->getDevice().createCommandPool(poolInfo);

    ServiceTable::commandPool.set(this);
}

talon::CommandPool::~CommandPool() {
    ServiceTable::commandPool.clear(this);
    ServiceTable::deviceManager->getDevice().destroyCommandPool(commandPool);
}

std::vector<vk::CommandBuffer> talon::CommandPool::createCommandBuffers(uint32_t count, vk::CommandBufferLevel level) const {
    vk::CommandBufferAllocateInfo allocInfo = {};
    allocInfo.commandPool = commandPool;
    allocInfo.level = level;
    allocInfo.commandBufferCount = count;
    return ServiceTable::deviceManager->getDevice().allocateCommandBuffers(allocInfo);
}

void talon::CommandPool::destroyCommandBuffers(const std::vector<vk::CommandBuffer> &commandBuffers) const {
    ServiceTable::deviceManager->getDevice().freeCommandBuffers(commandPool, commandBuffers);
}
