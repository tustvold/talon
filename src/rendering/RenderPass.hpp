#pragma once
#include <vulkan/vulkan.hpp>
#include "TalonConfig.hpp"

TALON_NS_BEGIN

class SwapChain;
class DeviceManager;

class RenderPass {
public:
    RenderPass(const SwapChain* swapChain, DeviceManager* deviceManager);
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

    void createRenderPass(const SwapChain *swapChain, DeviceManager *pManager);
    void createFrameBuffers(const SwapChain *swapChain, DeviceManager *pManager);

};


TALON_NS_END


