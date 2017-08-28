#pragma once
#include <vulkan/vulkan.hpp>
#include "TalonConfig.hpp"

TALON_NS_BEGIN

class SwapChain;
class DeviceManager;
class CommandBuffer;
class Material;
class MaterialPipelineCache;

class RenderPass {
public:
    explicit RenderPass(SwapChain* swapChain);
    RenderPass(const RenderPass&) = delete;
    RenderPass(RenderPass&&) = delete;
    ~RenderPass();

    vk::RenderPass getRenderPass() const {
        return renderPass;
    }

    const std::vector<vk::Framebuffer>& getFramebuffers() const {
        return framebuffers;
    }

    void bindMaterial(SwapChain* swapChain, Material *pMaterial, CommandBuffer* buffer);

private:
    std::unique_ptr<MaterialPipelineCache> materialPipelineCache;
    std::vector<vk::Framebuffer> framebuffers;
    vk::RenderPass renderPass;

    void createRenderPass(SwapChain* swapChain);
    void createFrameBuffers(SwapChain* swapChain);
};


TALON_NS_END


