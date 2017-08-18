#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "SurfaceManager.hpp"

talon::SurfaceManager::SurfaceManager(InstanceManager* instanceManager,
                                      WindowManager* windowManager)
     {
    auto psurf = VkSurfaceKHR();
    if (glfwCreateWindowSurface(instanceManager->getInstance(), windowManager->getWindow(), nullptr, &psurf) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }

    surface = psurf;
}
