#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <rendering/singleton/DeviceManager.hpp>

TALON_NS_BEGIN

class MockDeviceManager : public DeviceManager {
public:
    MOCK_CONST_METHOD0(getGraphicsQueue, vk::Queue());
    MOCK_CONST_METHOD0(getPresentQueue, vk::Queue());
    MOCK_CONST_METHOD0(waitDeviceIdle, void());
    MOCK_CONST_METHOD1(createFramebuffer, vk::Framebuffer(vk::FramebufferCreateInfo &info));
    MOCK_CONST_METHOD1(destroyFramebuffer, void(vk::Framebuffer framebuffer));
    MOCK_CONST_METHOD1(createRenderPass, vk::RenderPass(const vk::RenderPassCreateInfo &info));
    MOCK_CONST_METHOD1(destroyRenderPass, void(vk::RenderPass renderPass));
    MOCK_CONST_METHOD1(createImageView, vk::ImageView(const vk::ImageViewCreateInfo &info));
    MOCK_CONST_METHOD1(destroyImageView, void(vk::ImageView imageView));
    MOCK_CONST_METHOD1(createSwapchainKHR, vk::SwapchainKHR(const vk::SwapchainCreateInfoKHR &info));
    MOCK_CONST_METHOD1(destroySwapchainKHR, void(vk::SwapchainKHR swapchainKHR));
    MOCK_CONST_METHOD1(getSwapchainImagesKHR, std::vector<vk::Image>(vk::SwapchainKHR swapchainKHR));
    MOCK_CONST_METHOD4(acquireNextImageKHR, vk::ResultValue<uint32_t>(vk::SwapchainKHR swapchain,
                                                  uint64_t timeout,
                                                  vk::Semaphore semaphore,
                                                  vk::Fence fence));
    MOCK_CONST_METHOD5(acquireNextImageKHR, vk::Result(vk::SwapchainKHR swapchain,
                                   uint64_t timeout,
                                   vk::Semaphore semaphore,
                                   vk::Fence fence,
                                   uint32_t *pImageIndex));
    MOCK_CONST_METHOD1(createCommandPool, vk::CommandPool(const vk::CommandPoolCreateInfo &info));
    MOCK_CONST_METHOD1(destroyCommandPool, void(vk::CommandPool commandPool));
    MOCK_CONST_METHOD1(allocateCommandBuffers, std::vector<vk::CommandBuffer>(const vk::CommandBufferAllocateInfo &info));
    MOCK_CONST_METHOD3(freeCommandBuffers, void(vk::CommandPool commandPool,
                            uint32_t commandBufferCount,
                            const vk::CommandBuffer *pCommandBuffers));
    MOCK_CONST_METHOD2(freeCommandBuffers, void(vk::CommandPool commandPool,
                            vk::ArrayProxy<const vk::CommandBuffer> commandBuffers));
    MOCK_CONST_METHOD1(createSemaphore, vk::Semaphore(const vk::SemaphoreCreateInfo &info));
    MOCK_CONST_METHOD1(destroySemaphore, void(vk::Semaphore semaphore));
    MOCK_CONST_METHOD1(createShaderModule, vk::ShaderModule(const vk::ShaderModuleCreateInfo &info));
    MOCK_CONST_METHOD1(destroyShaderModule, void(vk::ShaderModule module));
    MOCK_CONST_METHOD1(createPipelineLayout, vk::PipelineLayout(const vk::PipelineLayoutCreateInfo &info));
    MOCK_CONST_METHOD1(destroyPipelineLayout, void(vk::PipelineLayout layout));
    MOCK_CONST_METHOD2(createGraphicsPipeline, vk::Pipeline(const vk::PipelineCache &cache,
                                        vk::GraphicsPipelineCreateInfo &info));
    MOCK_CONST_METHOD1(destroyPipeline, void(vk::Pipeline pipeline));
    MOCK_CONST_METHOD1(createDescriptorSetLayout, vk::DescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo &info));
};

TALON_NS_END