#pragma once
#include <TalonConfig.hpp>
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class CommandPool {
    friend class CommandBuffer;
private:
    virtual void destroyCommandBuffers(const std::vector<vk::CommandBuffer>& commandBuffers) const = 0;
    virtual std::vector<vk::CommandBuffer> createCommandBuffers(uint32_t count, vk::CommandBufferLevel level) const = 0;

    virtual void destroyCommandBuffer(vk::CommandBuffer buffer) const = 0;
    virtual vk::CommandBuffer createCommandBuffer(vk::CommandBufferLevel level) = 0;
};

TALON_NS_END