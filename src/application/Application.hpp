#pragma once
#include <application/DebugCallback.hpp>
#include <rendering/system/SwapChain.hpp>
#include "TalonConfig.hpp"
#include "vulkan/vulkan.hpp"

TALON_NS_BEGIN
struct ApplicationInitSettings;

class VulkanWindowManager;
class VulkanInstanceManager;
class VulkanSurfaceManager;
class VulkanDeviceManager;
class VulkanMemoryAllocator;
class VulkanCommandPool;
class GameLoop;
class Scene;

class Application {
public:
    void run(std::unique_ptr<Scene>&& scene);

    explicit Application(const ApplicationInitSettings& initSettings);

    ~Application();

private:
    std::unique_ptr<VulkanWindowManager> windowManager;
    std::unique_ptr<VulkanInstanceManager> instanceManager;
    std::unique_ptr<DebugCallback> debugCallback;
    std::unique_ptr<VulkanSurfaceManager> surfaceManager;
    std::unique_ptr<VulkanDeviceManager> deviceManager;
    std::unique_ptr<VulkanMemoryAllocator> memoryAllocator;
    std::unique_ptr<VulkanCommandPool> commandPool;

    std::unique_ptr<Scene> currentScene;
    std::unique_ptr<SwapChain> swapChain;

    void vulkanDebugCallback(const VDebugCallbackArgs& args);

    void recreateSwapChain();
    void onWindowResized(vk::Extent2D extent2D);
};

TALON_NS_END