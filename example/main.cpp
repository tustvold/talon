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
    auto delegate = std::make_unique<MyApplicationDelegate>();
    auto app = std::make_unique<Application>(std::move(delegate));

    try {
        app->run();
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}