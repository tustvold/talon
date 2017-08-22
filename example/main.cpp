#include "vulkan/vulkan.hpp"
#include "Logging.hpp"
#include "Application.hpp"
#include "ApplicationInitSettings.hpp"

struct MyApplicationDelegate : public talon::ApplicationDelegate {
public:
    void vulkanDebugCallback(const talon::VDebugCallbackArgs& args) override {
        TLOGERROR("Validation Layer - %s\n", args.pMessage);
    }
};

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
        auto delegate = std::make_unique<MyApplicationDelegate>();
        auto app = std::make_unique<TALON_NS::Application>(std::move(delegate), settings);

        app->run();
    } catch (const std::runtime_error &e) {
        TLOGERROR("Runtime Error - %s", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}