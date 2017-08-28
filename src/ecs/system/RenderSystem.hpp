#pragma once
#include <rendering/system/CommandBuffer.hpp>
#include "TalonConfig.hpp"
#include "WorldConfig.hpp"
#include "DescriptorConfig.hpp"

TALON_NS_BEGIN

class SwapChain;
class RenderPass;

struct RenderSystemArgs {
    const World* world;
    DescriptorPool* descriptorPool;
    SwapChain* swapChain;
    RenderPass* renderPass;
    PrimaryCommandBuffer* primaryCommandBuffer;
    vk::CommandBufferInheritanceInfo* commandBufferInheritanceInfo;

    RenderSystemArgs(World *world, DescriptorPool* descriptorPool, SwapChain *swapChain, RenderPass *renderPass, PrimaryCommandBuffer* primaryCommandBuffer, vk::CommandBufferInheritanceInfo* commandBufferInheritanceInfo);
};

class RenderSystem {
public:
    virtual ~RenderSystem() = default;

    virtual void update(const RenderSystemArgs& args) = 0;
};

TALON_NS_END