#include "Application.hpp"
#include "GameLoop.hpp"

#include "rendering/DeviceManager.hpp"
#include "rendering/InstanceManager.hpp"
#include "rendering/WindowManager.hpp"
#include "rendering/SurfaceManager.hpp"
#include "rendering/CommandPool.hpp"
#include "rendering/MemoryAllocator.hpp"


USING_TALON_NS;

void Application::onWindowResized(vk::Extent2D extent2D) {
    recreateSwapChain();
}

void Application::run() {
    while (windowManager->poll()) {
        if (!gameLoop->renderFrame(deviceManager.get(), surfaceManager.get()))
            recreateSwapChain();
    }

    vkDeviceWaitIdle(deviceManager->getDevice());
}

void Application::recreateSwapChain() {
    deviceManager->getDevice().waitIdle();
    gameLoop.reset();
    gameLoop = std::make_unique<GameLoop>(deviceManager.get(), surfaceManager.get(), windowManager.get());
}

Application::Application(std::unique_ptr<ApplicationDelegate> &&delegate, const ApplicationInitSettings &initSettings)
    : applicationDelegate(std::move(delegate)) {

    windowManager = std::make_unique<WindowManager>(initSettings);
    instanceManager = std::make_unique<InstanceManager>(initSettings);

    if (initSettings.validationLayersEnabled)
        debugCallback = std::make_unique<DebugCallback>(Gallant::MakeDelegate(applicationDelegate.get(), &ApplicationDelegate::vulkanDebugCallback));

    surfaceManager = std::make_unique<SurfaceManager>(instanceManager.get(), windowManager.get());
    deviceManager = std::make_unique<DeviceManager>(initSettings, instanceManager.get(), surfaceManager.get());
    memoryAllocator = std::make_unique<MemoryAllocator>(deviceManager.get());
    commandPool = std::make_unique<CommandPool>(deviceManager.get(), surfaceManager.get());
    gameLoop = std::make_unique<GameLoop>(deviceManager.get(), surfaceManager.get(), windowManager.get());

    windowManager->getWindowResizeEvent().Connect(this, &Application::onWindowResized);
}

Application::~Application() {
    gameLoop.reset();

    memoryAllocator.reset();

    commandPool.reset();

    deviceManager.reset();

    debugCallback.reset();
    surfaceManager.reset();
    instanceManager.reset();
    windowManager.reset();
}