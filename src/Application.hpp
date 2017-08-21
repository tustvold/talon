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
#include "RenderLoop.hpp"

struct GLFWwindow;


TALON_NS_BEGIN

class Application {
public:
    void run();

    explicit Application(std::unique_ptr<ApplicationDelegate> &&delegate, const ApplicationInitSettings& initSettings);

    ~Application();

private:
    vk::DebugReportCallbackEXT callback;

    std::unique_ptr<ApplicationDelegate> applicationDelegate;

    std::unique_ptr<WindowManager> windowManager;
    std::unique_ptr<InstanceManager> instanceManager;
    std::unique_ptr<SurfaceManager> surfaceManager;
    std::unique_ptr<DeviceManager> deviceManager;
    std::unique_ptr<MemoryAllocator> memoryAllocator;

    std::unique_ptr<CommandPool> commandPool;

    std::unique_ptr<RenderLoop> renderLoop;

    void recreateSwapChain();

    void setupDebugCallback(const ApplicationInitSettings& initSettings);

    void onWindowResized(GLFWwindow *window, vk::Extent2D extent);

    static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugReportFlagsEXT flags,
                                                              VkDebugReportObjectTypeEXT objectType,
                                                              uint64_t object,
                                                              size_t location,
                                                              int32_t messageCode,
                                                              const char *pLayerPrefix,
                                                              const char *pMessage,
                                                              void *pUserData);
};

TALON_NS_END