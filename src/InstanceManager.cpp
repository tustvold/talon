#include <GLFW/glfw3.h>
#include "InstanceManager.hpp"
#include "vulkan_ext.hpp"

static bool checkValidationLayerSupport(const talon::ApplicationInitSettings& initSettings) {
    auto availableLayers = vk::enumerateInstanceLayerProperties();

    for (const char *layerName : initSettings.validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

static std::vector<const char *> getRequiredExtensions(const talon::ApplicationInitSettings& initSettings) {
    std::vector<const char *> extensions;

    unsigned int glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }

    if (initSettings.validationLayersEnabled) {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}

talon::InstanceManager::InstanceManager(const ApplicationInitSettings& initSettings) {
    if (initSettings.validationLayersEnabled && !checkValidationLayerSupport(initSettings)) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    vk::ApplicationInfo applicationInfo
        ("Hello Triangle", VK_MAKE_VERSION(1, 0, 0), "Talon", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0);

    vk::InstanceCreateInfo createInfo;
    createInfo.setPApplicationInfo(&applicationInfo);

    auto extensions = getRequiredExtensions(initSettings);
    createInfo.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()));
    createInfo.setPpEnabledExtensionNames(extensions.data());

    if (initSettings.validationLayersEnabled) {
        createInfo.setEnabledLayerCount(static_cast<uint32_t>(initSettings.validationLayers.size()));
        createInfo.setPpEnabledLayerNames(initSettings.validationLayers.data());
    } else {
        createInfo.setEnabledLayerCount(0);
    }
    instance = vk::createInstance(createInfo);
    vke::InitInstance(instance);
}

talon::InstanceManager::~InstanceManager() {
    instance.destroy();
}
