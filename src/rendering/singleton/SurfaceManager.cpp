#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <rendering/singleton/RenderServiceTable.hpp>
#include "SurfaceManager.hpp"
#include "InstanceManager.hpp"
#include "WindowManager.hpp"

USING_TALON_NS;

SurfaceManager::SurfaceManager(InstanceManager *instanceManager, WindowManager *windowManager) {
    auto psurf = VkSurfaceKHR();
    if (glfwCreateWindowSurface(instanceManager->getInstance(), windowManager->getWindow(), nullptr, &psurf)
        != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }

    surface = psurf;

    RenderServiceTable::surfaceManager.set(this);
}

SurfaceManager::~SurfaceManager() {
    RenderServiceTable::surfaceManager.clear(this);
}
