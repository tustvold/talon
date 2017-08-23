#include "RenderSystem.hpp"

USING_TALON_NS;

RenderSystemArgs::RenderSystemArgs(World *world,
                                   SwapChain *swapChain,
                                   RenderPass *renderPass,
                                   PrimaryCommandBuffer *primaryCommandBuffer,
                                   vk::CommandBufferInheritanceInfo *commandBufferInheritanceInfo)
    : world(world),
      swapChain(swapChain),
      renderPass(renderPass),
      primaryCommandBuffer(primaryCommandBuffer),
      commandBufferInheritanceInfo(commandBufferInheritanceInfo) {

}
