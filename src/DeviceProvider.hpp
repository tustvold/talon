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

protected:
    virtual ~DeviceProvider() = default;

    virtual vk::Device getDevice() const = 0;
};

class NullDeviceProvider : public DeviceProvider {
public:
    ~NullDeviceProvider() final = default;

    vk::Device getDevice() const final {
        TASSERT(false);
        return vk::Device();
    }
};