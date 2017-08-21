#pragma once
#include "TalonConfig.hpp"
#include "vulkan/vulkan.hpp"
#include "ApplicationDelegate.hpp"

TALON_NS_BEGIN
struct ApplicationInitSettings;

class WindowManager;
class InstanceManager;
class SurfaceManager;
class DeviceManager;
class MemoryAllocator;
class CommandPool;
class GameLoop;

class Application {
public:
    void run();

    explicit Application(std::unique_ptr<ApplicationDelegate> &&delegate, const ApplicationInitSettings& initSettings);

    ~Application();

private:
    std::unique_ptr<DebugCallback> debugCallback;
    std::unique_ptr<ApplicationDelegate> applicationDelegate;

    std::unique_ptr<WindowManager> windowManager;
    std::unique_ptr<InstanceManager> instanceManager;
    std::unique_ptr<SurfaceManager> surfaceManager;
    std::unique_ptr<DeviceManager> deviceManager;
    std::unique_ptr<MemoryAllocator> memoryAllocator;

    std::unique_ptr<CommandPool> commandPool;

    std::unique_ptr<GameLoop> gameLoop;

    void recreateSwapChain();

    void onWindowResized(vk::Extent2D extent);
};

TALON_NS_END