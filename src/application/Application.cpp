#include "Application.hpp"
#include "GameLoop.hpp"
#include "Scene.hpp"

#include "rendering/singleton/DeviceManager.hpp"
#include "rendering/singleton/InstanceManager.hpp"
#include "rendering/singleton/WindowManager.hpp"
#include "rendering/singleton/SurfaceManager.hpp"
#include "rendering/singleton/MemoryAllocator.hpp"
#include "rendering/singleton/CommandPool.hpp"


USING_TALON_NS;
void Application::run(std::unique_ptr<Scene>&& scene) {
    currentScene = std::move(scene);
    while (windowManager->poll()) {
        currentScene->renderFrame();
    }

    vkDeviceWaitIdle(deviceManager->getDevice());
}

void Application::vulkanDebugCallback(const VDebugCallbackArgs &args) {
    TLOGERROR("Validation Layer - %s\n", args.pMessage);
}

Application::Application(const ApplicationInitSettings &initSettings) {
    windowManager = std::make_unique<WindowManager>(initSettings);
    instanceManager = std::make_unique<InstanceManager>(initSettings);

    if (initSettings.validationLayersEnabled)
        debugCallback = std::make_unique<DebugCallback>(Gallant::MakeDelegate(this, &Application::vulkanDebugCallback));

    surfaceManager = std::make_unique<SurfaceManager>(instanceManager.get(), windowManager.get());
    deviceManager = std::make_unique<DeviceManager>(initSettings, instanceManager.get(), surfaceManager.get());
    memoryAllocator = std::make_unique<MemoryAllocator>(deviceManager.get());
    commandPool = std::make_unique<CommandPool>(deviceManager.get(), surfaceManager.get());
}

Application::~Application() {
    currentScene.reset();
    memoryAllocator.reset();
    commandPool.reset();
    deviceManager.reset();
    debugCallback.reset();
    surfaceManager.reset();
    instanceManager.reset();
    windowManager.reset();
}


