#include <GLFW/glfw3.h>
#include "GameLoop.hpp"
#include "ServiceTable.hpp"
#include "rendering/Vertex.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/SwapChain.hpp"
#include "rendering/Material.hpp"
#include "rendering/RenderPass.hpp"
#include "rendering/DeviceManager.hpp"
#include "rendering/MemoryAllocator.hpp"

USING_TALON_NS;

const std::vector<Vertex> test_vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

GameLoop::GameLoop(DeviceManager *deviceManager, SurfaceManager *surfaceManager, WindowManager *windowManager)
    : commandBuffers(1), updateCount(0), updateStartInterval(glfwGetTime()) {
    swapChain = std::make_unique<SwapChain>(windowManager, surfaceManager, deviceManager);
    renderPass = std::make_unique<RenderPass>(swapChain.get(), deviceManager);
    testMaterial = std::make_unique<Material>(swapChain.get(), renderPass.get(), deviceManager);

    auto meshData = makeMeshData(test_vertices);
    testMesh = std::make_unique<Mesh>(meshData);
}

GameLoop::~GameLoop() = default;

bool GameLoop::renderFrame(DeviceManager *deviceManager, SurfaceManager *surfaceManager) {
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

    deviceManager->getGraphicsQueue().waitIdle();

    uint32_t imageIndex;

    vk::Result result = deviceManager->getDevice().acquireNextImageKHR(swapChain->getSwapChain(),
                                                                       std::numeric_limits<uint64_t>::max(),
                                                                       imageAvailableSemaphore.get(),
                                                                       vk::Fence(),
                                                                       &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR) {
        return false;
    } else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    recordCommandBuffer(imageIndex);

    vk::SubmitInfo submitInfo = {};

    vk::Semaphore renderStartSemaphores[] = {imageAvailableSemaphore.get()};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;

    submitInfo.setPWaitSemaphores(renderStartSemaphores);
    submitInfo.setPWaitDstStageMask(waitStages);

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = commandBuffers.data();

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

void GameLoop::recordCommandBuffer(int index) {
    auto commandBuffer = commandBuffers[0];

    vk::CommandBufferBeginInfo beginInfo = {};

    commandBuffer.reset(vk::CommandBufferResetFlags());

    commandBuffer.begin(beginInfo);

    vk::RenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.renderPass = renderPass->getRenderPass();
    renderPassInfo.framebuffer = renderPass->getFramebuffers()[index];
    renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
    renderPassInfo.renderArea.extent = swapChain->getExtents();

    vk::ClearValue clearColor;
    clearColor.setColor(vk::ClearColorValue());
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.setPClearValues(&clearColor);

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, testMaterial->getGraphicsPipeline());

    testMesh->bind(commandBuffer);

    commandBuffer.draw(testMesh->getNumVertices(), 1, 0, 0);

    commandBuffer.endRenderPass();
    commandBuffer.end();
}

