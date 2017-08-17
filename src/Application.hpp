#pragma once
#include "vulkan/vulkan.hpp"
#include "ApplicationDelegate.hpp"

struct GLFWwindow;

struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete() {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

class Application {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    explicit Application(std::unique_ptr<ApplicationDelegate> &&delegate)
        : window(nullptr),
          swapChainImageFormat(vk::Format::eUndefined),
          applicationDelegate(std::move(delegate)) {

    }

private:
    GLFWwindow *window;

    vk::Instance instance;
    vk::DebugReportCallbackEXT callback;
    vk::SurfaceKHR surface;

    vk::PhysicalDevice physicalDevice;
    vk::Device device;

    vk::Queue graphicsQueue;
    vk::Queue presentQueue;

    vk::SwapchainKHR swapChain;
    std::vector<vk::Image> swapChainImages;
    vk::Format swapChainImageFormat;
    vk::Extent2D swapChainExtent;
    std::vector<vk::ImageView> swapChainImageViews;
    std::vector<vk::Framebuffer> swapChainFramebuffers;

    vk::RenderPass renderPass;
    vk::PipelineLayout pipelineLayout;
    vk::Pipeline graphicsPipeline;

    vk::CommandPool commandPool;

    vk::Buffer vertexBuffer;
    vk::DeviceMemory vertexBufferMemory;

    std::vector<vk::CommandBuffer> commandBuffers;

    vk::Semaphore imageAvailableSemaphore;
    vk::Semaphore renderFinishedSemaphore;

    std::unique_ptr<ApplicationDelegate> applicationDelegate;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanupSwapChain();

    void cleanup();

    void recreateSwapChain();

    void createInstance();

    void setupDebugCallback();

    void createSurface();

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSwapChain();

    void createImageViews();

    void createRenderPass();

    void createGraphicsPipeline();

    void createFrameBuffers();

    void createCommandPool();

    void createVertexBuffer();

    uint32_t findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags &properties);

    void createCommandBuffers();

    void createSemaphores();

    void drawFrame();

    vk::ShaderModule createShaderModule(const std::vector<char> &code);

    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);

    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);

    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities);

    SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device);

    bool isDeviceSuitable(vk::PhysicalDevice device);

    bool checkDeviceExtensionSupport(vk::PhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);

    std::vector<const char *> getRequiredExtensions();

    bool checkValidationLayerSupport();

    static std::vector<char> readFile(const std::string &filename);

    static void onWindowResized(GLFWwindow *window, int width, int height);

    static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugReportFlagsEXT flags,
                                                              VkDebugReportObjectTypeEXT objectType,
                                                              uint64_t object,
                                                              size_t location,
                                                              int32_t messageCode,
                                                              const char *pLayerPrefix,
                                                              const char *pMessage,
                                                              void *pUserData);
};
