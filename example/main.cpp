#include "vulkan/vulkan.hpp"
#include <iostream>
#include <fstream>
#include <set>
#include <Application.hpp>

struct MyApplicationDelegate : public ApplicationDelegate {
public:
    VkBool32 vulkanDebugCallback(VkDebugReportFlagsEXT flags,
                                 VkDebugReportObjectTypeEXT objectType,
                                 uint64_t object,
                                 size_t location,
                                 int32_t messageCode,
                                 const char *pLayerPrefix,
                                 const char *pMessage) override {
        std::cerr << "validation layer: " << pMessage << std::endl;

        return VK_FALSE;
    }

};

int main() {

    talon::ApplicationInitSettings settings;
    settings.windowWidth = 800;
    settings.windowHeight = 600;
    settings.windowName = "Hello World";
    settings.deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
#ifndef NDEBUG
    settings.validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");
    settings.validationLayersEnabled = true;
#else
    settings.validationLayersEnabled = false;
#endif

    try {
        auto delegate = std::make_unique<MyApplicationDelegate>();
        auto app = std::make_unique<TALON_NS::Application>(std::move(delegate), settings);

        app->run();
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}