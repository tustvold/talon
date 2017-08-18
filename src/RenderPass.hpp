#pragma once
#include <vulkan/vulkan.hpp>
#include "talon.hpp"
#include "SwapChain.hpp"

TALON_NS_BEGIN

class RenderPass {
public:
    RenderPass(const SwapChain* swapChain, const std::shared_ptr<DeviceManager>& deviceManager);

    ~RenderPass();

    vk::RenderPass getRenderPass() const {
        return renderPass;
    }

    const std::vector<vk::Framebuffer>& getFramebuffers() const {
        return framebuffers;
    }

private:
    vk::RenderPass renderPass;
    std::vector<vk::Framebuffer> framebuffers;

    std::shared_ptr<DeviceManager> deviceManager;

    void createRenderPass(const SwapChain* swapChain);
    void createFrameBuffers(const SwapChain* swapChain);

};


TALON_NS_END


