#include "talon.hpp"

#include "Application.hpp"
#include "vulkan_ext.hpp"
#include "Vertex.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <fstream>
#include <set>

USING_TALON_NS;

const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

VkBool32 Application::vulkanDebugCallback(VkDebugReportFlagsEXT flags,
                                          VkDebugReportObjectTypeEXT objectType,
                                          uint64_t object,
                                          size_t location,
                                          int32_t messageCode,
                                          const char *pLayerPrefix,
                                          const char *pMessage,
                                          void *pUserData) {

    auto app = reinterpret_cast<Application *>(pUserData);
    return app->applicationDelegate->vulkanDebugCallback(flags,
                                                         objectType,
                                                         object,
                                                         location,
                                                         messageCode,
                                                         pLayerPrefix,
                                                         pMessage);
}

void Application::onWindowResized(GLFWwindow *window, vk::Extent2D extent2D) {
    recreateSwapChain();
}

void Application::run() {
    mainLoop();
}

void Application::mainLoop() {
    while (!glfwWindowShouldClose(windowManager->getWindow())) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(deviceManager->getDevice());
}

void Application::cleanupSwapChain() {
    deviceManager->getDevice().freeCommandBuffers(commandPool, commandBuffers);
    material.reset();
    renderPass.reset();
    swapChain.reset();
}

void Application::cleanup() {
    cleanupSwapChain();

    deviceManager->getDevice().destroyBuffer(vertexBuffer);
    deviceManager->getDevice().freeMemory(vertexBufferMemory);

    deviceManager->getDevice().destroySemaphore(renderFinishedSemaphore);
    deviceManager->getDevice().destroySemaphore(imageAvailableSemaphore);

    deviceManager->getDevice().destroyCommandPool(commandPool);

    deviceManager.reset();

    instanceManager->getInstance().destroyDebugReportCallbackEXT(callback);
    surfaceManager.reset();
    instanceManager.reset();
    windowManager.reset();
}

void Application::recreateSwapChain() {
    deviceManager->getDevice().waitIdle();

    cleanupSwapChain();

    swapChain = std::make_shared<SwapChain>(windowManager.get(), surfaceManager.get(), deviceManager);
    renderPass = std::make_shared<RenderPass>(swapChain.get(), deviceManager);

    material = std::make_shared<Material>(swapChain.get(), renderPass.get(), deviceManager);
    createCommandBuffers();
}

void Application::setupDebugCallback(const ApplicationInitSettings &initSettings) {
    if (!initSettings.validationLayersEnabled) return;

    vk::DebugReportCallbackCreateInfoEXT createInfo;
    createInfo.setFlags(
        vk::Flags(vk::DebugReportFlagBitsEXT::eError) | vk::DebugReportFlagBitsEXT::eWarning);
    createInfo.setPfnCallback(vulkanDebugCallback);
    createInfo.pUserData = this;

    callback = instanceManager->getInstance().createDebugReportCallbackEXT(createInfo);
}

void Application::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = deviceManager->getQueueFamilyIndices(surfaceManager.get());

    vk::CommandPoolCreateInfo poolInfo = {};
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndices.graphicsFamily);

    commandPool = deviceManager->getDevice().createCommandPool(poolInfo);
}

void Application::createVertexBuffer() {
    vk::BufferCreateInfo bufferInfo = {};
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    vertexBuffer = deviceManager->getDevice().createBuffer(bufferInfo);

    vk::MemoryRequirements memRequirements = deviceManager->getDevice().getBufferMemoryRequirements(vertexBuffer);

    vk::MemoryAllocateInfo allocInfo = {};
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                               vk::Flags(vk::MemoryPropertyFlagBits::eHostVisible)
                                                   | vk::MemoryPropertyFlagBits::eHostCoherent);
    vertexBufferMemory = deviceManager->getDevice().allocateMemory(allocInfo);

    deviceManager->getDevice().bindBufferMemory(vertexBuffer, vertexBufferMemory, 0);

    void *data;
    deviceManager->getDevice().mapMemory(vertexBufferMemory, 0, bufferInfo.size, vk::MemoryMapFlags(), &data);
    memcpy(data, vertices.data(), (size_t) bufferInfo.size);
    deviceManager->getDevice().unmapMemory(vertexBufferMemory);
}

uint32_t Application::findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags &properties) {
    auto memProperties = deviceManager->getPhysicalDevice().getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type at "  S_LOCATION);
}

void Application::createSemaphores() {
    vk::SemaphoreCreateInfo semaphoreInfo = {};

    imageAvailableSemaphore = deviceManager->getDevice().createSemaphore(semaphoreInfo);
    renderFinishedSemaphore = deviceManager->getDevice().createSemaphore(semaphoreInfo);
}

void Application::drawFrame() {
    // Update App State

    auto current = glfwGetTime();
    auto delta = current - updateStartInterval;
    if (delta > 5) {
        updateStartInterval = current;

        TLOGINFO("Average time is %f ms, framerate is %f", 1000 * delta / (double)updateCount, (double)updateCount / delta);

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
        recreateSwapChain();
        return;
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
        recreateSwapChain();
    } else if (result != vk::Result::eSuccess) {
        throw std::runtime_error("failed to present swap chain image!");
    }
}

void Application::createCommandBuffers() {
    auto size = renderPass->getFramebuffers().size();
    commandBuffers.resize(size);

    vk::CommandBufferAllocateInfo allocInfo = {};
    allocInfo.commandPool = commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    commandBuffers = deviceManager->getDevice().allocateCommandBuffers(allocInfo);
}

void Application::recordCommandBuffer(int index) {
    auto commandBuffer = &commandBuffers[0];

    vk::CommandBufferBeginInfo beginInfo = {};
    //beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

    commandBuffer->reset(vk::CommandBufferResetFlags());

    commandBuffer->begin(beginInfo);

    vk::RenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.renderPass = renderPass->getRenderPass();
    renderPassInfo.framebuffer = renderPass->getFramebuffers()[index];
    renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
    renderPassInfo.renderArea.extent = swapChain->getExtents();

    vk::ClearValue clearColor;
    clearColor.setColor(vk::ClearColorValue());
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.setPClearValues(&clearColor);

    commandBuffer->beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, material->getGraphicsPipeline());

    vk::Buffer vertexBuffers[] = {vertexBuffer};
    vk::DeviceSize offsets[] = {0};

    commandBuffer->bindVertexBuffers(0, 1, vertexBuffers, offsets);

    commandBuffer->draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0);

    commandBuffer->endRenderPass();
    commandBuffer->end();
}

Application::Application(std::unique_ptr<ApplicationDelegate> &&delegate, const ApplicationInitSettings &initSettings)
    : applicationDelegate(std::move(delegate)), updateCount(0), updateStartInterval(glfwGetTime())

       {

    windowManager = std::make_shared<WindowManager>(initSettings);
    instanceManager = std::make_shared<InstanceManager>(initSettings);

    setupDebugCallback(initSettings);

    surfaceManager = std::make_shared<SurfaceManager>(instanceManager.get(), windowManager.get());
    deviceManager = std::make_shared<DeviceManager>(initSettings, instanceManager.get(), surfaceManager.get());

    swapChain = std::make_shared<SwapChain>(windowManager.get(), surfaceManager.get(), deviceManager);
    renderPass = std::make_shared<RenderPass>(swapChain.get(), deviceManager);
    material = std::make_shared<Material>(swapChain.get(), renderPass.get(), deviceManager);
    createCommandPool();
    createVertexBuffer();
    createCommandBuffers();
    createSemaphores();

    windowManager->getWindowResizeEvent().Connect(this, &Application::onWindowResized);
}

