#pragma once
#include "TalonConfig.hpp"
#include "WorldConfig.hpp"

TALON_NS_BEGIN

class SwapChain;
class RenderPass;

struct RenderSystemArgs {
    const World* world;
    SwapChain* swapChain;
    RenderPass* renderPass;
    vk::CommandBuffer commandBuffer;

    RenderSystemArgs(World *world, SwapChain *swapChain, RenderPass *renderPass, const vk::CommandBuffer &commandBuffer);
};

class RenderSystem {
public:
    virtual ~RenderSystem() = default;

    virtual void update(const RenderSystemArgs& args) = 0;
};

TALON_NS_END