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

struct GLFWwindow;


TALON_NS_BEGIN

class Application {
public:
    void run();

    explicit Application(std::unique_ptr<ApplicationDelegate> &&delegate, const ApplicationInitSettings& initSettings);

    ~Application() {
        cleanup();
    }

private:
    vk::DebugReportCallbackEXT callback;

    std::unique_ptr<Buffer> vertexBuffer;

    vk::Semaphore imageAvailableSemaphore;
    vk::Semaphore renderFinishedSemaphore;

    std::unique_ptr<ApplicationDelegate> applicationDelegate;

    std::unique_ptr<WindowManager> windowManager;
    std::unique_ptr<InstanceManager> instanceManager;
    std::unique_ptr<SurfaceManager> surfaceManager;
    std::unique_ptr<DeviceManager> deviceManager;
    std::unique_ptr<MemoryAllocator> memoryAllocator;

    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<RenderPass> renderPass;
    std::unique_ptr<Material> material;

    vk::CommandPool commandPool;
    std::vector<vk::CommandBuffer> commandBuffers;


    uint32_t updateCount;
    double updateStartInterval;

    void mainLoop();

    void cleanupSwapChain();

    void cleanup();

    void recreateSwapChain();

    void setupDebugCallback(const ApplicationInitSettings& initSettings);

    void createCommandPool();

    void createCommandBuffers();

    void createVertexBuffer();

    uint32_t findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags &properties);

    void createSemaphores();

    void drawFrame();

    void onWindowResized(GLFWwindow *window, vk::Extent2D extent);

    static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugReportFlagsEXT flags,
                                                              VkDebugReportObjectTypeEXT objectType,
                                                              uint64_t object,
                                                              size_t location,
                                                              int32_t messageCode,
                                                              const char *pLayerPrefix,
                                                              const char *pMessage,
                                                              void *pUserData);

    void recordCommandBuffer(int index);
};

TALON_NS_END