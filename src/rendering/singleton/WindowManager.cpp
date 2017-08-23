#include <GLFW/glfw3.h>
#include <rendering/singleton/RenderServiceTable.hpp>
#include "WindowManager.hpp"

USING_TALON_NS;

WindowManager::WindowManager(const ApplicationInitSettings &initSettings) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(initSettings.windowWidth,
                              initSettings.windowHeight,
                              initSettings.windowName,
                              nullptr,
                              nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, WindowManager::onWindowResizedStatic);

    RenderServiceTable::windowManager.set(this);
}

WindowManager::~WindowManager() {
    RenderServiceTable::windowManager.clear(this);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowManager::onWindowResizedStatic(GLFWwindow *window, int width, int height) {
    auto *manager = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    manager->onWindowResized(window, width, height);
}

void WindowManager::onWindowResized(GLFWwindow *window, int width, int height) {
    windowResizeEvent(vk::Extent2D(static_cast<uint32_t>(width), static_cast<uint32_t>(height)));
}

vk::Extent2D WindowManager::getWindowExtents() const {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    vk::Extent2D extent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };
    return extent;
}
bool WindowManager::poll() {
    if (glfwWindowShouldClose(window))
        return false;
    glfwPollEvents();
    return true;
}

