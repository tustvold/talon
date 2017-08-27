#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <rendering/singleton/RenderServiceTable.hpp>
#include "VulkanSurfaceManager.hpp"
#include "rendering/singleton/impl/VulkanInstanceManager.hpp"
#include "VulkanWindowManager.hpp"

USING_TALON_NS;

VulkanSurfaceManager::VulkanSurfaceManager(VulkanInstanceManager *instanceManager, VulkanWindowManager *windowManager) {
    auto psurf = VkSurfaceKHR();
    if (glfwCreateWindowSurface(instanceManager->getInstance(), windowManager->getWindow(), nullptr, &psurf)
        != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }

    surface = psurf;

    RenderServiceTable::surfaceManager.set(this);
}

VulkanSurfaceManager::~VulkanSurfaceManager() {
    RenderServiceTable::surfaceManager.clear(this);
}
