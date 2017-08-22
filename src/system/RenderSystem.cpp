#include "RenderSystem.hpp"

USING_TALON_NS;

RenderSystemArgs::RenderSystemArgs(World *world,
                                   SwapChain *swapChain,
                                   RenderPass *renderPass,
                                   const vk::CommandBuffer &commandBuffer)
    : world(world), swapChain(swapChain), renderPass(renderPass), commandBuffer(commandBuffer) {}