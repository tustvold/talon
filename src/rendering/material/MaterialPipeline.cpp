#include <rendering/singleton/RenderServiceTable.hpp>
#include "MaterialPipeline.hpp"
#include "rendering/singleton/DeviceManager.hpp"
#include <rendering/system/CommandBuffer.hpp>

USING_TALON_NS;

MaterialPipeline::MaterialPipeline(vk::PipelineLayout pipelineLayout, vk::Pipeline pipeline)
    : pipelineLayout(pipelineLayout), pipeline(pipeline) {}



MaterialPipeline::~MaterialPipeline() {
    RenderServiceTable::deviceManager->destroyPipeline(pipeline);
    RenderServiceTable::deviceManager->destroyPipelineLayout(pipelineLayout);
}

void MaterialPipeline::bind(CommandBuffer *buffer) {
    buffer->bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
}



