#include "CommandPool.hpp"
#include "rendering/singleton/DeviceManager.hpp"
#include "rendering/singleton/SurfaceManager.hpp"
#include "RenderServiceTable.hpp"

USING_TALON_NS;

CommandPool::CommandPool(DeviceManager *deviceManager, SurfaceManager* surfaceManager) {
    QueueFamilyIndices queueFamilyIndices = deviceManager->getQueueFamilyIndices(surfaceManager);

    vk::CommandPoolCreateInfo poolInfo = {};
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndices.graphicsFamily);

    commandPool = deviceManager->getDevice().createCommandPool(poolInfo);

    RenderServiceTable::commandPool.set(this);
}

CommandPool::~CommandPool() {
    RenderServiceTable::commandPool.clear(this);
    RenderServiceTable::deviceManager->getDevice().destroyCommandPool(commandPool);
}

std::vector<vk::CommandBuffer> CommandPool::createCommandBuffers(uint32_t count, vk::CommandBufferLevel level) const {
    vk::CommandBufferAllocateInfo allocInfo = {};
    allocInfo.commandPool = commandPool;
    allocInfo.level = level;
    allocInfo.commandBufferCount = count;
    return RenderServiceTable::deviceManager->getDevice().allocateCommandBuffers(allocInfo);
}

void CommandPool::destroyCommandBuffers(const std::vector<vk::CommandBuffer> &commandBuffers) const {
    RenderServiceTable::deviceManager->getDevice().freeCommandBuffers(commandPool, commandBuffers);
}
