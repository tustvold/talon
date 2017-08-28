#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>
#include <vulkan_mem_alloc.hpp>
#include "rendering/singleton/DeviceManager.hpp"

TALON_NS_BEGIN
struct ApplicationInitSettings;
class VulkanInstanceManager;
class VulkanSurfaceManager;

struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete() {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

class VulkanDeviceManager : public DeviceManager {
public:
    explicit VulkanDeviceManager(const ApplicationInitSettings &initSettings, VulkanInstanceManager *instanceManager,
                                 VulkanSurfaceManager *surfaceManager);

    ~VulkanDeviceManager() final;

    vk::Queue getGraphicsQueue() const final {
        return graphicsQueue_;
    }

    vk::Queue getPresentQueue() const final {
        return presentQueue_;
    }

    QueueFamilyIndices getQueueFamilyIndices(const VulkanSurfaceManager *surfaceManager) const {
        return getQueueFamilyIndices(surfaceManager, physicalDevice_);
    }

    SwapChainSupportDetails getSwapChainSupportDetails(const VulkanSurfaceManager *surfaceManager) const {
        return getSwapChainSupportDetails(surfaceManager, physicalDevice_);
    }

    void waitDeviceIdle() const final {
        device_.waitIdle();
    }

    vk::Framebuffer createFramebuffer(vk::FramebufferCreateInfo &info) const final {
        return device_.createFramebuffer(info);
    }

    void destroyFramebuffer(vk::Framebuffer framebuffer) const final {
        device_.destroyFramebuffer(framebuffer);
    }

    vk::RenderPass createRenderPass(const vk::RenderPassCreateInfo &info) const final {
        return device_.createRenderPass(info);
    }

    void destroyRenderPass(vk::RenderPass renderPass) const final {
        device_.destroyRenderPass(renderPass);
    }

    vk::ImageView createImageView(const vk::ImageViewCreateInfo &info) const final {
        return device_.createImageView(info);
    }

    void destroyImageView(vk::ImageView imageView) const final {
        device_.destroyImageView(imageView);
    }

    vk::SwapchainKHR createSwapchainKHR(const vk::SwapchainCreateInfoKHR &info) const final {
        return device_.createSwapchainKHR(info);
    }

    void destroySwapchainKHR(vk::SwapchainKHR swapchainKHR) const final {
        device_.destroySwapchainKHR(swapchainKHR);
    }

    std::vector<vk::Image> getSwapchainImagesKHR(vk::SwapchainKHR swapchainKHR) const final {
        return device_.getSwapchainImagesKHR(swapchainKHR);
    }

    vk::ResultValue<uint32_t> acquireNextImageKHR(vk::SwapchainKHR swapchain,
                                                  uint64_t timeout,
                                                  vk::Semaphore semaphore,
                                                  vk::Fence fence) const final {
        return device_.acquireNextImageKHR(swapchain, timeout, semaphore, fence);
    }

    vk::Result acquireNextImageKHR(vk::SwapchainKHR swapchain,
                                   uint64_t timeout,
                                   vk::Semaphore semaphore,
                                   vk::Fence fence,
                                   uint32_t *pImageIndex) const final {
        return device_.acquireNextImageKHR(swapchain, timeout, semaphore, fence, pImageIndex);
    }

    vk::CommandPool createCommandPool(const vk::CommandPoolCreateInfo &info) const final {
        return device_.createCommandPool(info);
    }

    void destroyCommandPool(vk::CommandPool commandPool) const final {
        device_.destroyCommandPool(commandPool);
    }

    std::vector<vk::CommandBuffer> allocateCommandBuffers(const vk::CommandBufferAllocateInfo &info) const final {
        return device_.allocateCommandBuffers(info);
    }

    void freeCommandBuffers(vk::CommandPool commandPool,
                            uint32_t commandBufferCount,
                            const vk::CommandBuffer *pCommandBuffers) const final {
        device_.freeCommandBuffers(commandPool, commandBufferCount, pCommandBuffers);
    }

    void freeCommandBuffers(vk::CommandPool commandPool, vk::ArrayProxy<const vk::CommandBuffer> commandBuffers) const final {
        device_.freeCommandBuffers(commandPool, commandBuffers);
    }

    vk::Semaphore createSemaphore(const vk::SemaphoreCreateInfo &info) const final {
        return device_.createSemaphore(info);
    }

    void destroySemaphore(vk::Semaphore semaphore) const final {
        return device_.destroySemaphore(semaphore);
    }

    vk::ShaderModule createShaderModule(const vk::ShaderModuleCreateInfo &info) const final {
        return device_.createShaderModule(info);
    }

    void destroyShaderModule(vk::ShaderModule module) const final {
        device_.destroyShaderModule(module);
    }

    vk::PipelineLayout createPipelineLayout(const vk::PipelineLayoutCreateInfo &info) const final {
        return device_.createPipelineLayout(info);
    }

    void destroyPipelineLayout(vk::PipelineLayout layout) const final {
        device_.destroyPipelineLayout(layout);
    }

    vk::Pipeline createGraphicsPipeline(vk::PipelineCache cache, const vk::GraphicsPipelineCreateInfo &info) const final {
        return device_.createGraphicsPipeline(cache, info);
    }

    void destroyPipeline(vk::Pipeline pipeline) const final {
        device_.destroyPipeline(pipeline);
    }

    vk::DescriptorSetLayout createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo &info) const final {
        return device_.createDescriptorSetLayout(info);
    }

    void destroyDescriptorSetLayout(vk::DescriptorSetLayout layout) const final {
        device_.destroyDescriptorSetLayout(layout);
    }

    vk::Device getDevice() const {
        return device_;
    }

    vk::PhysicalDevice getPhysicalDevice() const {
        return physicalDevice_;
    }

    vk::DescriptorPool createDescriptorPool(const vk::DescriptorPoolCreateInfo &info) const final {
        return device_.createDescriptorPool(info);
    }

    void destroyDescriptorPool(vk::DescriptorPool pool) const final {
        device_.destroyDescriptorPool(pool);
    }

    void allocateDescriptorSets(const vk::DescriptorSetAllocateInfo& info, vk::DescriptorSet* sets) const final {
        device_.allocateDescriptorSets(&info, sets);
    }

    void freeDescriptorSets(vk::DescriptorPool descriptorPool, vk::ArrayProxy<const vk::DescriptorSet> descriptorSets) const final {
        device_.freeDescriptorSets(descriptorPool, descriptorSets);
    }

private:
    vk::Device device_;
    vk::PhysicalDevice physicalDevice_;
    vk::Queue graphicsQueue_;
    vk::Queue presentQueue_;

    void pickPhysicalDevice(const ApplicationInitSettings &initSettings,
                            VulkanInstanceManager *pManager,
                            VulkanSurfaceManager *pSurfaceManager);

    void createLogicalDevice(const ApplicationInitSettings &initSettings, const VulkanSurfaceManager *surfaceManager);

    static bool isDeviceSuitable(const ApplicationInitSettings &initSettings,
                                 const VulkanSurfaceManager *surfaceManager,
                                 vk::PhysicalDevice device);

    static QueueFamilyIndices getQueueFamilyIndices(const VulkanSurfaceManager *surfaceManager, vk::PhysicalDevice device);
    static SwapChainSupportDetails getSwapChainSupportDetails(const VulkanSurfaceManager *surfaceManager,
                                                              vk::PhysicalDevice device);
    static bool checkDeviceExtensionSupport(const ApplicationInitSettings &initSettings, vk::PhysicalDevice device);
};

TALON_NS_END


