#pragma once
#include <vulkan/vulkan.hpp>
#include "TalonConfig.hpp"
#include "rendering/material/Material.hpp"
#include "rendering/material/MaterialPipelineCache.hpp"

TALON_NS_BEGIN

class SwapChain;
class DeviceManager;

class RenderPass {
public:
    explicit RenderPass(SwapChain* swapChain);
    ~RenderPass();

    vk::RenderPass getRenderPass() const {
        return renderPass;
    }

    const std::vector<vk::Framebuffer>& getFramebuffers() const {
        return framebuffers;
    }

    void bindMaterial(talon::Material *pMaterial, vk::CommandBuffer buffer);

private:
    SwapChain* swapChain;
    vk::RenderPass renderPass;
    MaterialPipelineCache materialPipelineCache;
    std::vector<vk::Framebuffer> framebuffers;

    void createRenderPass();
    void createFrameBuffers();
};


TALON_NS_END


