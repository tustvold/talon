#pragma once
#include "TalonConfig.hpp"
#include "vulkan/vulkan.hpp"
#include "ApplicationDelegate.hpp"
#include "InstanceManager.hpp"
#include "DeviceManager.hpp"
#include "WindowManager.hpp"
#include "SurfaceManager.hpp"
#include "SwapChain.hpp"
#include "RenderPass.hpp"
#include "Material.hpp"
#include "MemoryAllocator.hpp"
#include "CommandPool.hpp"
#include "Mesh.hpp"
#include "GameLoop.hpp"
#include "DebugCallback.hpp"

struct GLFWwindow;


TALON_NS_BEGIN

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

    void onWindowResized(GLFWwindow *window, vk::Extent2D extent);
};

TALON_NS_END