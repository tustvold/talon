#pragma once
#include <vulkan/vulkan.hpp>
#include "TalonConfig.hpp"

TALON_NS_BEGIN

class CommandBuffer {
public:
    virtual ~CommandBuffer();

    void reset(const vk::CommandBufferResetFlags& flags) {
        commandBuffer.reset(flags);
    }

    void reset() {
        reset({});
    }

    void begin(const vk::CommandBufferBeginInfo& info) {
        commandBuffer.begin(info);
    }

    void end() {
        commandBuffer.end();
    }

    void bindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const vk::Buffer* pBuffers, const vk::DeviceSize* pOffsets) {
        commandBuffer.bindVertexBuffers(firstBinding, bindingCount, pBuffers, pOffsets);
    }

    void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
        commandBuffer.draw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void bindPipeline(vk::PipelineBindPoint pipelineBindPoint, vk::Pipeline pipeline) {
        commandBuffer.bindPipeline(pipelineBindPoint, pipeline);
    }

protected:
    explicit CommandBuffer(vk::CommandBufferLevel level);

    vk::CommandBuffer commandBuffer;
};

class SecondaryCommandBuffer : public CommandBuffer {
    friend class PrimaryCommandBuffer;
public:
    SecondaryCommandBuffer() : CommandBuffer(vk::CommandBufferLevel::eSecondary) {

    }
};

class PrimaryCommandBuffer : public CommandBuffer {
public:
    PrimaryCommandBuffer() : CommandBuffer(vk::CommandBufferLevel::ePrimary) {

    }

    auto& get() {
        return commandBuffer;
    }

    void beginRenderPass(const vk::RenderPassBeginInfo& info, vk::SubpassContents contents) {
        commandBuffer.beginRenderPass(info, contents);
    }

    void endRenderPass() {
        commandBuffer.endRenderPass();
    }

    void executeCommandBuffer(SecondaryCommandBuffer* buffer) {
        commandBuffer.executeCommands(1, &buffer->commandBuffer);
    }
};



TALON_NS_END