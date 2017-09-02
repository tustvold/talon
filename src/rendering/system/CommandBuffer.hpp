#pragma once
#include <vulkan/vulkan.hpp>
#include "TalonConfig.hpp"

TALON_NS_BEGIN

struct CommandBufferRecordHandle {
    explicit CommandBufferRecordHandle(vk::CommandBuffer buffer, const vk::CommandBufferBeginInfo &info)
        : buffer(buffer) {
        buffer.begin(info);
    }

    CommandBufferRecordHandle(const CommandBufferRecordHandle &) = delete;
    CommandBufferRecordHandle(CommandBufferRecordHandle &&) = delete;

    ~CommandBufferRecordHandle() {
        buffer.end();
    }

private:
    vk::CommandBuffer buffer;
};

struct RenderPassRecordHandle {
    explicit RenderPassRecordHandle(vk::CommandBuffer buffer,
                                    const vk::RenderPassBeginInfo &info,
                                    vk::SubpassContents contents) : buffer(buffer) {
        buffer.beginRenderPass(info, contents);
    }

    RenderPassRecordHandle(const RenderPassRecordHandle &) = delete;
    RenderPassRecordHandle(RenderPassRecordHandle &&) = delete;

    ~RenderPassRecordHandle() {
        buffer.endRenderPass();
    }

private:
    vk::CommandBuffer buffer;
};

class CommandBuffer {
public:
    virtual ~CommandBuffer();

    void reset(const vk::CommandBufferResetFlags &flags) {
        commandBuffer.reset(flags);
    }

    void reset() {
        reset({});
    }

    CommandBufferRecordHandle begin(const vk::CommandBufferBeginInfo &info) {
        return CommandBufferRecordHandle(commandBuffer, info);
    }

    void bindVertexBuffers(uint32_t firstBinding,
                           uint32_t bindingCount,
                           const vk::Buffer *pBuffers,
                           const vk::DeviceSize *pOffsets) {
        commandBuffer.bindVertexBuffers(firstBinding, bindingCount, pBuffers, pOffsets);
    }

    void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
        commandBuffer.draw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void bindPipeline(vk::PipelineBindPoint pipelineBindPoint, vk::Pipeline pipeline) {
        commandBuffer.bindPipeline(pipelineBindPoint, pipeline);
    }

    CommandBuffer(const CommandBuffer &) = delete;
    CommandBuffer(CommandBuffer &&) = delete;

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

    auto &get() {
        return commandBuffer;
    }

    RenderPassRecordHandle beginRenderPass(const vk::RenderPassBeginInfo &info, vk::SubpassContents contents) {
        return RenderPassRecordHandle(commandBuffer, info, contents);
    }

    void executeCommandBuffer(SecondaryCommandBuffer *buffer) {
        commandBuffer.executeCommands(1, &buffer->commandBuffer);
    }
};

TALON_NS_END