#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>
#include <rendering/singleton/CommandPool.hpp>

TALON_NS_BEGIN

class VulkanDeviceManager;
class VulkanSurfaceManager;

class VulkanCommandPool : public CommandPool {
public:
    explicit VulkanCommandPool(VulkanDeviceManager *deviceManager, VulkanSurfaceManager *surfaceManager);
    ~VulkanCommandPool();

private:
    vk::CommandPool commandPool;

    void destroyCommandBuffers(const std::vector<vk::CommandBuffer>& commandBuffers) const final;
    std::vector<vk::CommandBuffer> createCommandBuffers(uint32_t count, vk::CommandBufferLevel level) const final;

    void destroyCommandBuffer(vk::CommandBuffer buffer) const final;
    vk::CommandBuffer createCommandBuffer(vk::CommandBufferLevel level) final;

};

TALON_NS_END