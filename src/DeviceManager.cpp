#include <set>
#include "DeviceManager.hpp"
#include "InstanceManager.hpp"
#include "SurfaceManager.hpp"

USING_TALON_NS;

QueueFamilyIndices DeviceManager::getQueueFamilyIndices(const SurfaceManager *surfaceManager,
                                                        vk::PhysicalDevice device) {
    QueueFamilyIndices indices;
    auto queueFamilies = device.getQueueFamilyProperties();

    uint32_t i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        vk::Bool32 presentSupport = VK_FALSE;
        device.getSurfaceSupportKHR(i, surfaceManager->getSurface(), &presentSupport);

        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }
    return indices;
}

SwapChainSupportDetails DeviceManager::getSwapChainSupportDetails(const SurfaceManager *surfaceManager,
                                                                  vk::PhysicalDevice device) {
    SwapChainSupportDetails details;
    auto surface = surfaceManager->getSurface();

    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);

    return details;
}

bool DeviceManager::checkDeviceExtensionSupport(const ApplicationInitSettings &initSettings,
                                                vk::PhysicalDevice device) {
    auto availableExtensions = device.enumerateDeviceExtensionProperties();
    std::set<std::string>
        requiredExtensions(initSettings.deviceExtensions.begin(), initSettings.deviceExtensions.end());

    for (const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

DeviceManager::DeviceManager(const ApplicationInitSettings &initSettings, InstanceManager *instanceManager,
                             SurfaceManager *surfaceManager) {
    pickPhysicalDevice(initSettings, instanceManager, surfaceManager);
    createLogicalDevice(initSettings, surfaceManager);
}

DeviceManager::~DeviceManager() {
    device_.destroy();
}

bool DeviceManager::isDeviceSuitable(const ApplicationInitSettings &initSettings,
                                     const SurfaceManager *surfaceManager,
                                     vk::PhysicalDevice device) {
    QueueFamilyIndices indices = getQueueFamilyIndices(surfaceManager, device);

    bool extensionsSupported = checkDeviceExtensionSupport(initSettings, device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = getSwapChainSupportDetails(surfaceManager, device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

void DeviceManager::pickPhysicalDevice(const ApplicationInitSettings &initSettings,
                                       InstanceManager *instanceManager,
                                       SurfaceManager *surfaceManager) {
    auto devices = instanceManager->getInstance().enumeratePhysicalDevices();

    for (const auto &device : devices) {
        if (isDeviceSuitable(initSettings, surfaceManager, device)) {
            physicalDevice_ = device;
            break;
        }
    }

    TASSERT(physicalDevice_);
}

void DeviceManager::createLogicalDevice(const ApplicationInitSettings &initSettings,
                                        const SurfaceManager *surfaceManager) {
    auto deviceQueueFamilyIndices_ = getQueueFamilyIndices(surfaceManager, physicalDevice_);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<int>
        uniqueQueueFamilies = {deviceQueueFamilyIndices_.graphicsFamily, deviceQueueFamilyIndices_.presentFamily};

    float queuePriority = 1.0f;
    for (auto queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.setQueueFamilyIndex((uint32_t) queueFamily);
        queueCreateInfo.setQueueCount(1);
        queueCreateInfo.setPQueuePriorities(&queuePriority);
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures;

    vk::DeviceCreateInfo createInfo = {};

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.setPEnabledFeatures(&deviceFeatures);

    createInfo.enabledExtensionCount = static_cast<uint32_t>(initSettings.deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = initSettings.deviceExtensions.data();

    if (initSettings.validationLayersEnabled) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(initSettings.validationLayers.size());
        createInfo.ppEnabledLayerNames = initSettings.validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    device_ = physicalDevice_.createDevice(createInfo);

    graphicsQueue_ = device_.getQueue(static_cast<uint32_t>(deviceQueueFamilyIndices_.graphicsFamily), 0);
    presentQueue_ = device_.getQueue(static_cast<uint32_t>(deviceQueueFamilyIndices_.presentFamily), 0);
}


