#pragma once
#include <rendering/singleton/DebugCallback.hpp>
#include "TalonConfig.hpp"
#include "vulkan/vulkan.hpp"

TALON_NS_BEGIN
struct ApplicationInitSettings;

class WindowManager;
class InstanceManager;
class SurfaceManager;
class DeviceManager;
class MemoryAllocator;
class CommandPool;
class GameLoop;
class Scene;

class Application {
public:
    void run(std::unique_ptr<Scene>&& scene);

    explicit Application(const ApplicationInitSettings& initSettings);

    ~Application();

private:
    std::unique_ptr<WindowManager> windowManager;
    std::unique_ptr<InstanceManager> instanceManager;
    std::unique_ptr<DebugCallback> debugCallback;
    std::unique_ptr<SurfaceManager> surfaceManager;
    std::unique_ptr<DeviceManager> deviceManager;
    std::unique_ptr<MemoryAllocator> memoryAllocator;
    std::unique_ptr<CommandPool> commandPool;

    std::unique_ptr<Scene> currentScene;

    void vulkanDebugCallback(const VDebugCallbackArgs& args);
};

TALON_NS_END