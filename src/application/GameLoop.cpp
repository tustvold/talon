#include <GLFW/glfw3.h>
#include <ecs/system/GameSystemUpdateTransforms.hpp>
#include <ecs/system/RenderSystemMeshFilter.hpp>
#include "GameLoop.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "../../example/TestVertex.hpp"
#include "rendering/mesh/Mesh.hpp"
#include "rendering/system/SwapChain.hpp"
#include "rendering/material/Material.hpp"
#include "rendering/system/RenderPass.hpp"
#include "rendering/singleton/DeviceManager.hpp"
#include "rendering/singleton/MemoryAllocator.hpp"

USING_TALON_NS;

GameLoop::GameLoop()
    : updateCount(0), updateStartInterval(glfwGetTime()) {

}

void GameLoop::addRenderSystem(std::unique_ptr<RenderSystem> &&renderSystem) {
    renderSystems.emplace_back(std::move(renderSystem));
}

void GameLoop::addGameSystem(std::unique_ptr<GameSystem> &&gameSystem) {
    gameSystems.emplace_back(std::move(gameSystem));
}

GameLoop::~GameLoop() = default;

bool GameLoop::doUpdate(World &world, SwapChain* swapChain) {
    auto deviceManager = RenderServiceTable::deviceManager;

    auto current = glfwGetTime();
    auto delta = current - updateStartInterval;
    if (delta > 5) {
        updateStartInterval = current;

        TLOGINFO("Average time is %f ms, framerate is %f",
                 1000 * delta / (double) updateCount,
                 (double) updateCount / delta);

        updateCount = 0;
    }
    updateCount++;

    for (auto &system : gameSystems) {
        system->update(world);
    }

    deviceManager->getGraphicsQueue().waitIdle();

    uint32_t imageIndex;

    vk::Result result = deviceManager->acquireNextImageKHR(swapChain->getSwapChain(),
                                                                       std::numeric_limits<uint64_t>::max(),
                                                                       imageAvailableSemaphore.get(),
                                                                       vk::Fence(),
                                                                       &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR) {
        return false;
    } else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    recordCommandBuffer(world, swapChain, imageIndex);

    vk::SubmitInfo submitInfo = {};

    vk::Semaphore renderStartSemaphores[] = {imageAvailableSemaphore.get()};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;

    submitInfo.setPWaitSemaphores(renderStartSemaphores);
    submitInfo.setPWaitDstStageMask(waitStages);

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer.get();

    vk::Semaphore renderFinishedSemaphores[] = {renderFinishedSemaphore.get()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.setPSignalSemaphores(renderFinishedSemaphores);

    deviceManager->getGraphicsQueue().submit(1, &submitInfo, vk::Fence());

    vk::PresentInfoKHR presentInfo = {};

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.setPWaitSemaphores(renderFinishedSemaphores);

    vk::SwapchainKHR swapChains[] = {swapChain->getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.setPSwapchains(swapChains);

    presentInfo.setPImageIndices(&imageIndex);

    result = deviceManager->getPresentQueue().presentKHR(presentInfo);

    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
        return false;
    } else if (result != vk::Result::eSuccess) {
        throw std::runtime_error("failed to present swap chain image!");
    }
    return true;
}

void GameLoop::recordCommandBuffer(World &world, SwapChain* swapChain, int index) {

    vk::CommandBufferBeginInfo beginInfo = {};

    commandBuffer.reset(vk::CommandBufferResetFlags());

    commandBuffer.begin(beginInfo);

    auto renderPass = swapChain->getRenderPass(0);

    vk::RenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.renderPass = renderPass->getRenderPass();
    renderPassInfo.framebuffer = renderPass->getFramebuffers()[index];
    renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
    renderPassInfo.renderArea.extent = swapChain->getExtents();

    vk::ClearValue clearColor;
    clearColor.setColor(vk::ClearColorValue());
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.setPClearValues(&clearColor);

    vk::CommandBufferInheritanceInfo inheritanceInfo;
    inheritanceInfo.renderPass = renderPassInfo.renderPass;
    inheritanceInfo.framebuffer = renderPassInfo.framebuffer;

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eSecondaryCommandBuffers);

    RenderSystemArgs args(&world, &descriptorPool, swapChain, renderPass, &commandBuffer, &inheritanceInfo);

    for (auto &system : renderSystems) {
        system->update(args);
    }

    commandBuffer.endRenderPass();
    commandBuffer.end();
}

