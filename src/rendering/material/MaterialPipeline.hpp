#pragma once
#include <vulkan/vulkan.hpp>
#include "TalonConfig.hpp"

TALON_NS_BEGIN

class CommandBuffer;

class MaterialPipeline {
public:
    MaterialPipeline(vk::PipelineLayout pipelineLayout, vk::Pipeline pipeline);
    MaterialPipeline(const MaterialPipeline& pipeline) = delete;
    MaterialPipeline(MaterialPipeline&& pipeline) = delete;
    ~MaterialPipeline();

    void bind(CommandBuffer *buffer);

private:
    vk::PipelineLayout pipelineLayout;
    vk::Pipeline pipeline;
};

TALON_NS_END