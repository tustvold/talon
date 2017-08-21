#pragma once
#include "TalonConfig.hpp"
#include "ApplicationInitSettings.hpp"
#include "SurfaceManager.hpp"
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN
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

class DeviceManager {
public:
    explicit DeviceManager(const ApplicationInitSettings& initSettings, InstanceManager* instanceManager,
                           SurfaceManager* surfaceManager);

    ~DeviceManager();

    vk::Device getDevice() const {
        return device_;
    }

    vk::PhysicalDevice getPhysicalDevice() const {
        return physicalDevice_;
    }

    vk::Queue getGraphicsQueue() const {
        return graphicsQueue_;
    }

    vk::Queue getPresentQueue() const {
        return presentQueue_;
    }

    auto getQueueFamilyIndices(const SurfaceManager *surfaceManager) {
        return getQueueFamilyIndices(surfaceManager, physicalDevice_);
    }

    auto getSwapChainSupportDetails(const SurfaceManager *surfaceManager) {
        return getSwapChainSupportDetails(surfaceManager, physicalDevice_);
    }

private:
    vk::Device device_;
    vk::PhysicalDevice physicalDevice_;
    vk::Queue graphicsQueue_;
    vk::Queue presentQueue_;

    void pickPhysicalDevice(const ApplicationInitSettings &initSettings,
                                InstanceManager *pManager,
                                SurfaceManager *pSurfaceManager);

    void createLogicalDevice(const ApplicationInitSettings& initSettings, const SurfaceManager *surfaceManager);
    static bool isDeviceSuitable(const ApplicationInitSettings& initSettings, const SurfaceManager* surfaceManager, vk::PhysicalDevice device);

    static QueueFamilyIndices getQueueFamilyIndices(const SurfaceManager *surfaceManager, vk::PhysicalDevice device);
    static SwapChainSupportDetails getSwapChainSupportDetails(const SurfaceManager *surfaceManager, vk::PhysicalDevice device);
    static bool checkDeviceExtensionSupport(const ApplicationInitSettings& initSettings, vk::PhysicalDevice device);
};


TALON_NS_END


