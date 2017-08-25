#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class DeviceManager;
class SurfaceManager;

class CommandPool {
    friend class CommandBuffer;
public:
    explicit CommandPool(DeviceManager *deviceManager, SurfaceManager *surfaceManager);
    ~CommandPool();

private:
    vk::CommandPool commandPool;

    void destroyCommandBuffers(const std::vector<vk::CommandBuffer>& commandBuffers) const;
    std::vector<vk::CommandBuffer> createCommandBuffers(uint32_t count, vk::CommandBufferLevel level) const;

    void destroyCommandBuffer(vk::CommandBuffer buffer) const;
    vk::CommandBuffer createCommandBuffer(vk::CommandBufferLevel level);

};

TALON_NS_END