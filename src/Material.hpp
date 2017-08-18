#pragma once
#include "talon.hpp"
#include "DeviceManager.hpp"
#include "SwapChain.hpp"
#include "RenderPass.hpp"
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class Material {
public:

    Material(const SwapChain* swapChain, const RenderPass* renderPass, const std::shared_ptr<DeviceManager>& deviceManager);

    ~Material();

    vk::Pipeline getGraphicsPipeline() const {
        return graphicsPipeline;
    }

private:
    vk::PipelineLayout pipelineLayout;
    vk::Pipeline graphicsPipeline;

    std::shared_ptr<DeviceManager> deviceManager;

    void createGraphicsPipeline(const SwapChain* swapChain, const RenderPass* renderPass);
    vk::ShaderModule createShaderModule(const std::vector<char> &code);
};

TALON_NS_END