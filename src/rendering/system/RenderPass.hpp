#pragma once
#include <vulkan/vulkan.hpp>
#include "TalonConfig.hpp"
#include "rendering/material/Material.hpp"
#include "rendering/material/MaterialPipelineCache.hpp"

TALON_NS_BEGIN

class SwapChain;
class DeviceManager;
class CommandBuffer;

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

    void bindMaterial(SwapChain* swapChain, talon::Material *pMaterial, CommandBuffer* buffer);

private:
    vk::RenderPass renderPass;
    MaterialPipelineCache materialPipelineCache;
    std::vector<vk::Framebuffer> framebuffers;

    void createRenderPass(SwapChain* swapChain);
    void createFrameBuffers(SwapChain* swapChain);
};


TALON_NS_END


