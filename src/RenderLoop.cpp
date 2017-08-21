#include <GLFW/glfw3.h>
#include "RenderLoop.hpp"
#include "ServiceTable.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "SwapChain.hpp"
#include "Material.hpp"
#include "RenderPass.hpp"

USING_TALON_NS;

const std::vector<Vertex> test_vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

RenderLoop::RenderLoop(DeviceManager *deviceManager, SurfaceManager* surfaceManager, WindowManager* windowManager) : updateCount(0), updateStartInterval(glfwGetTime()) {
    swapChain = std::make_unique<SwapChain>(windowManager, surfaceManager, deviceManager);
    renderPass = std::make_unique<RenderPass>(swapChain.get(), deviceManager);
    material = std::make_unique<Material>(swapChain.get(), renderPass.get(), deviceManager);

    vk::SemaphoreCreateInfo semaphoreInfo = {};
    imageAvailableSemaphore = deviceManager->getDevice().createSemaphore(semaphoreInfo);
    renderFinishedSemaphore = deviceManager->getDevice().createSemaphore(semaphoreInfo);

    commandBuffers = ServiceTable::commandPool->createCommandBuffers(1);

    auto meshData = makeMeshData(test_vertices);
    testMesh = std::make_unique<Mesh>(meshData);
}

RenderLoop::~RenderLoop() {
    ServiceTable::commandPool->destroyCommandBuffers(commandBuffers);
    ServiceTable::deviceProvider->destroySemaphore(renderFinishedSemaphore);
    ServiceTable::deviceProvider->destroySemaphore(imageAvailableSemaphore);
}

bool RenderLoop::renderFrame(DeviceManager *deviceManager, SurfaceManager *surfaceManager) {
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
                                                                       imageAvailableSemaphore,
                                                                       vk::Fence(),
                                                                       &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR) {
        return false;
    } else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    recordCommandBuffer(imageIndex);

    vk::SubmitInfo submitInfo = {};

    vk::Semaphore waitSemaphores[] = {imageAvailableSemaphore};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;

    submitInfo.setPWaitSemaphores(waitSemaphores);
    submitInfo.setPWaitDstStageMask(waitStages);

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[0];

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.setPSignalSemaphores(&renderFinishedSemaphore);

    deviceManager->getGraphicsQueue().submit(1, &submitInfo, vk::Fence());

    vk::PresentInfoKHR presentInfo = {};

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.setPWaitSemaphores(&renderFinishedSemaphore);

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

void RenderLoop::recordCommandBuffer(int index) {
    auto commandBuffer = commandBuffers[0];

    vk::CommandBufferBeginInfo beginInfo = {};
    //beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

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

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, material->getGraphicsPipeline());

    testMesh->bind(commandBuffer);

    commandBuffer.draw(testMesh->getNumVertices(), 1, 0, 0);

    commandBuffer.endRenderPass();
    commandBuffer.end();
}


