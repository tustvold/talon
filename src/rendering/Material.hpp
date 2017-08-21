#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class SwapChain;
class RenderPass;
class DeviceManager;

class Material {
public:

    Material(const SwapChain* swapChain, const RenderPass* renderPass, DeviceManager* deviceManager);
    ~Material();

    vk::Pipeline getGraphicsPipeline() const {
        return graphicsPipeline;
    }

private:
    vk::PipelineLayout pipelineLayout;
    vk::Pipeline graphicsPipeline;
    vk::PipelineBindPoint bindPoint;

    void createGraphicsPipeline(const SwapChain *swapChain, const RenderPass *renderPass, DeviceManager *pManager);
    vk::ShaderModule createShaderModule(std::vector<char> code, DeviceManager *pManager);
};

TALON_NS_END