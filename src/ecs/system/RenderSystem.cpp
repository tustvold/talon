#include "RenderSystem.hpp"

USING_TALON_NS;

RenderSystemArgs::RenderSystemArgs(World *world,
                                   DescriptorPool* descriptorPool,
                                   SwapChain *swapChain,
                                   RenderPass *renderPass,
                                   PrimaryCommandBuffer *primaryCommandBuffer,
                                   vk::CommandBufferInheritanceInfo *commandBufferInheritanceInfo)
    : world(world),
      descriptorPool(descriptorPool),
      swapChain(swapChain),
      renderPass(renderPass),
      primaryCommandBuffer(primaryCommandBuffer),
      commandBufferInheritanceInfo(commandBufferInheritanceInfo) {

}
