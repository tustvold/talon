#pragma once
#include "TalonConfig.hpp"
#include "DeviceManager.hpp"

TALON_NS_BEGIN

class CommandPool {
public:
    explicit CommandPool(DeviceManager *deviceManager, SurfaceManager *surfaceManager);
    ~CommandPool();

    void destroyCommandBuffers(const std::vector<vk::CommandBuffer>& commandBuffers) const;

    std::vector<vk::CommandBuffer> createCommandBuffers(uint32_t count,
                                                        vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary) const;

private:
    vk::CommandPool commandPool;
};

TALON_NS_END