#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>

struct VmaAllocatorCreateInfo;

TALON_NS_BEGIN

// Generated using
// Find Regex: ([^ ]*) ([^\W]*)(\([^)]*\)) const \{(\n[^}]*)*}
// Replace : virtual $1 $2$3 const = 0;

class DeviceManager {
public:
    virtual ~DeviceManager() = default;

    virtual vk::Queue getGraphicsQueue() const = 0;

    virtual vk::Queue getPresentQueue() const = 0;

    virtual void waitDeviceIdle() const = 0;

    virtual vk::Framebuffer createFramebuffer(vk::FramebufferCreateInfo &info) const = 0;

    virtual void destroyFramebuffer(vk::Framebuffer framebuffer) const = 0;

    virtual vk::RenderPass createRenderPass(const vk::RenderPassCreateInfo &info) const = 0;

    virtual void destroyRenderPass(vk::RenderPass renderPass) const = 0;

    virtual vk::ImageView createImageView(const vk::ImageViewCreateInfo &info) const = 0;

    virtual void destroyImageView(vk::ImageView imageView) const = 0;

    virtual vk::SwapchainKHR createSwapchainKHR(const vk::SwapchainCreateInfoKHR &info) const = 0;

    virtual void destroySwapchainKHR(vk::SwapchainKHR swapchainKHR) const = 0;

    virtual std::vector<vk::Image> getSwapchainImagesKHR(vk::SwapchainKHR swapchainKHR) const = 0;

    virtual vk::ResultValue<uint32_t> acquireNextImageKHR(vk::SwapchainKHR swapchain,
                                                  uint64_t timeout,
                                                  vk::Semaphore semaphore,
                                                  vk::Fence fence) const = 0;

    virtual vk::Result acquireNextImageKHR(vk::SwapchainKHR swapchain,
                                   uint64_t timeout,
                                   vk::Semaphore semaphore,
                                   vk::Fence fence,
                                   uint32_t *pImageIndex) const = 0;

    virtual vk::CommandPool createCommandPool(const vk::CommandPoolCreateInfo &info) const = 0;

    virtual void destroyCommandPool(vk::CommandPool commandPool) const = 0;

    virtual std::vector<vk::CommandBuffer> allocateCommandBuffers(const vk::CommandBufferAllocateInfo &info) const = 0;

    virtual void freeCommandBuffers(vk::CommandPool commandPool,
                            uint32_t commandBufferCount,
                            const vk::CommandBuffer *pCommandBuffers) const = 0;

    virtual void freeCommandBuffers(vk::CommandPool commandPool, vk::ArrayProxy<const vk::CommandBuffer> commandBuffers) const = 0;

    virtual vk::Semaphore createSemaphore(const vk::SemaphoreCreateInfo &info) const = 0;

    virtual void destroySemaphore(vk::Semaphore semaphore) const = 0;

    virtual vk::ShaderModule createShaderModule(const vk::ShaderModuleCreateInfo &info) const = 0;

    virtual void destroyShaderModule(vk::ShaderModule module) const = 0;

    virtual vk::PipelineLayout createPipelineLayout(const vk::PipelineLayoutCreateInfo &info) const = 0;

    virtual void destroyPipelineLayout(vk::PipelineLayout layout) const = 0;

    virtual vk::Pipeline createGraphicsPipeline(vk::PipelineCache cache, const vk::GraphicsPipelineCreateInfo &info) const = 0;

    virtual void destroyPipeline(vk::Pipeline pipeline) const = 0;

    virtual vk::DescriptorSetLayout createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo &info) const = 0;

    virtual void destroyDescriptorSetLayout(vk::DescriptorSetLayout layout) const = 0;

    virtual vk::DescriptorPool createDescriptorPool(const vk::DescriptorPoolCreateInfo& info) const = 0;

    virtual void destroyDescriptorPool(vk::DescriptorPool pool) const = 0;
};

TALON_NS_END
