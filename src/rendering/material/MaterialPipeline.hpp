#pragma once
#include <vulkan/vulkan.hpp>
#include "TalonConfig.hpp"

TALON_NS_BEGIN

class SwapChain;
class RenderPass;
class Material;

class MaterialPipeline {
public:
    MaterialPipeline(Material* material, SwapChain* swapChain, RenderPass* renderPass);
    ~MaterialPipeline();

    void bind(vk::CommandBuffer commandBuffer) {
        commandBuffer.bindPipeline(bindPoint, pipeline);
    }

private:
    vk::PipelineLayout pipelineLayout;
    vk::Pipeline pipeline;
    vk::PipelineBindPoint bindPoint;
};


TALON_NS_END