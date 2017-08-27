#include <GLFW/glfw3.h>
#include <rendering/singleton/RenderServiceTable.hpp>
#include "VulkanWindowManager.hpp"

USING_TALON_NS;

VulkanWindowManager::VulkanWindowManager(const ApplicationInitSettings &initSettings) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(initSettings.windowWidth,
                              initSettings.windowHeight,
                              initSettings.windowName,
                              nullptr,
                              nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, VulkanWindowManager::onWindowResizedStatic);

    RenderServiceTable::windowManager.set(this);
}

VulkanWindowManager::~VulkanWindowManager() {
    RenderServiceTable::windowManager.clear(this);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void VulkanWindowManager::onWindowResizedStatic(GLFWwindow *window, int width, int height) {
    auto *manager = reinterpret_cast<VulkanWindowManager *>(glfwGetWindowUserPointer(window));
    manager->onWindowResized(window, width, height);
}

void VulkanWindowManager::onWindowResized(GLFWwindow *window, int width, int height) {
    windowResizeEvent(vk::Extent2D(static_cast<uint32_t>(width), static_cast<uint32_t>(height)));
}

vk::Extent2D VulkanWindowManager::getWindowExtents() const {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    vk::Extent2D extent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };
    return extent;
}
bool VulkanWindowManager::poll() {
    if (glfwWindowShouldClose(window))
        return false;
    glfwPollEvents();
    return true;
}

