#pragma once

#include <vulkan/vulkan.hpp>
#include "Logging.hpp"
class DeviceProvider {
public:
    void destroyImageView(vk::ImageView imageView) const {
        getDevice().destroyImageView(imageView);
    }

    void destroySwapchainKHR(vk::SwapchainKHR swapchainKHR) const {
        getDevice().destroySwapchainKHR(swapchainKHR);
    }

    void destroyFramebuffer(vk::Framebuffer framebuffer) const {
        getDevice().destroyFramebuffer(framebuffer);
    }

    void destroyRenderPass(vk::RenderPass renderPass) const {
        getDevice().destroyRenderPass(renderPass);
    }

    void destroyPipeline(vk::Pipeline pipeline) const {
        getDevice().destroyPipeline(pipeline);
    }

    void destroyPipelineLayout(vk::PipelineLayout pipelineLayout) const {
        getDevice().destroyPipelineLayout(pipelineLayout);
    }

    void destroyCommandPool(vk::CommandPool commandPool) const {
        getDevice().destroyCommandPool(commandPool);
    }

    void destroySemaphore(vk::Semaphore semaphore) const {
        getDevice().destroySemaphore(semaphore);
    }

    void freeCommandBuffers(vk::CommandPool commandPool, vk::ArrayProxy<const vk::CommandBuffer> buffers) const {
        getDevice().freeCommandBuffers(commandPool, buffers);
    }

    std::vector<vk::CommandBuffer> allocateCommandBuffers(const vk::CommandBufferAllocateInfo& info) const {
        return getDevice().allocateCommandBuffers(info);
    }

protected:
    virtual ~DeviceProvider() = default;

    virtual vk::Device getDevice() const = 0;
};