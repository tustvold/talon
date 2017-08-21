#include "TalonConfig.hpp"

#include "Application.hpp"
#include "VulkanLoader.hpp"
#include "Vertex.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <fstream>
#include <set>

USING_TALON_NS;

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
    while (!glfwWindowShouldClose(windowManager->getWindow())) {
        glfwPollEvents();
        if (!renderLoop->renderFrame(deviceManager.get(), surfaceManager.get()))
            recreateSwapChain();
    }

    vkDeviceWaitIdle(deviceManager->getDevice());
}

void Application::recreateSwapChain() {
    deviceManager->getDevice().waitIdle();
    renderLoop.reset();
    renderLoop = std::make_unique<RenderLoop>(deviceManager.get(), surfaceManager.get(), windowManager.get());
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

Application::Application(std::unique_ptr<ApplicationDelegate> &&delegate, const ApplicationInitSettings &initSettings)
    : applicationDelegate(std::move(delegate)) {

    windowManager = std::make_unique<WindowManager>(initSettings);
    instanceManager = std::make_unique<InstanceManager>(initSettings);

    setupDebugCallback(initSettings);

    surfaceManager = std::make_unique<SurfaceManager>(instanceManager.get(), windowManager.get());
    deviceManager = std::make_unique<DeviceManager>(initSettings, instanceManager.get(), surfaceManager.get());
    memoryAllocator = std::make_unique<MemoryAllocator>(deviceManager.get());
    commandPool = std::make_unique<CommandPool>(deviceManager.get(), surfaceManager.get());
    renderLoop = std::make_unique<RenderLoop>(deviceManager.get(), surfaceManager.get(), windowManager.get());

    windowManager->getWindowResizeEvent().Connect(this, &Application::onWindowResized);
}

Application::~Application() {
    renderLoop.reset();

    memoryAllocator.reset();

    commandPool.reset();

    deviceManager.reset();

    instanceManager->getInstance().destroyDebugReportCallbackEXT(callback);
    surfaceManager.reset();
    instanceManager.reset();
    windowManager.reset();
}