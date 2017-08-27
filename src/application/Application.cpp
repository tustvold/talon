#include <rendering/singleton/impl/VulkanDeviceManager.hpp>
#include <rendering/singleton/impl/VulkanMemoryAllocator.hpp>
#include "rendering/singleton/impl/VulkanInstanceManager.hpp"
#include "rendering/singleton/impl/VulkanSurfaceManager.hpp"
#include "Application.hpp"
#include "GameLoop.hpp"
#include "Scene.hpp"


#include "rendering/singleton/impl/VulkanWindowManager.hpp"
#include "rendering/singleton/impl/VulkanCommandPool.hpp"


USING_TALON_NS;


void Application::vulkanDebugCallback(const VDebugCallbackArgs &args) {
    TLOGERROR("Validation Layer - %s\n", args.pMessage);
}

Application::Application(const ApplicationInitSettings &initSettings) {
    windowManager = std::make_unique<VulkanWindowManager>(initSettings);
    instanceManager = std::make_unique<VulkanInstanceManager>(initSettings);

    if (initSettings.validationLayersEnabled)
        debugCallback = std::make_unique<DebugCallback>(Gallant::MakeDelegate(this, &Application::vulkanDebugCallback));

    surfaceManager = std::make_unique<VulkanSurfaceManager>(instanceManager.get(), windowManager.get());
    deviceManager = std::make_unique<VulkanDeviceManager>(initSettings, instanceManager.get(), surfaceManager.get());
    memoryAllocator = std::make_unique<VulkanMemoryAllocator>(deviceManager.get());
    commandPool = std::make_unique<VulkanCommandPool>(deviceManager.get(), surfaceManager.get());

    windowManager->getWindowResizeEvent().Connect(this, &Application::onWindowResized);
}

Application::~Application() {
    windowManager->getWindowResizeEvent().Disconnect(this, &Application::onWindowResized);

    swapChain.reset();
    currentScene.reset();
    memoryAllocator.reset();
    commandPool.reset();
    deviceManager.reset();
    debugCallback.reset();
    surfaceManager.reset();
    instanceManager.reset();
    windowManager.reset();
}

void Application::run(std::unique_ptr<Scene>&& scene) {
    currentScene = std::move(scene);

    recreateSwapChain();
    while (windowManager->poll()) {
        if (!currentScene->renderFrame(swapChain.get())) {
            recreateSwapChain();
        }
    }
    deviceManager->waitDeviceIdle();
}


void Application::recreateSwapChain() {
    deviceManager->waitDeviceIdle();
    swapChain.reset();
    swapChain = std::make_unique<SwapChain>(deviceManager.get(), surfaceManager.get(), windowManager.get(), currentScene->getNumRenderPasses());
}

void Application::onWindowResized(vk::Extent2D extent2D) {
    recreateSwapChain();
}

