#include "vulkan/vulkan.hpp"
#include "util/Logging.hpp"
#include "application/Application.hpp"
#include "application/ApplicationInitSettings.hpp"
#include "TestScene.hpp"

USING_TALON_NS;

int main() {

    talon::ApplicationInitSettings settings;
    settings.windowWidth = 800;
    settings.windowHeight = 600;
    settings.windowName = "Simple Triangle";
    settings.deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
#ifndef NDEBUG
    settings.validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");
    settings.validationLayersEnabled = true;
#else
    settings.validationLayersEnabled = false;
#endif
    try {
        auto app = std::make_unique<Application>(settings);
        app->run(std::make_unique<TestScene>());
    } catch (const std::runtime_error &e) {
        TLOGERROR("Runtime Error - %s", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}