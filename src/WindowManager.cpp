#include <GLFW/glfw3.h>
#include "WindowManager.hpp"

talon::WindowManager::WindowManager(const ApplicationInitSettings &initSettings) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(initSettings.windowWidth,
                              initSettings.windowHeight,
                              initSettings.windowName,
                              nullptr,
                              nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, WindowManager::onWindowResizedStatic);
}

talon::WindowManager::~WindowManager() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void talon::WindowManager::onWindowResizedStatic(GLFWwindow *window, int width, int height) {
    auto *manager = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    manager->onWindowResized(window, width, height);
}

void talon::WindowManager::onWindowResized(GLFWwindow *window, int width, int height) {

    windowResizeEvent(window, vk::Extent2D(static_cast<uint32_t>(width), static_cast<uint32_t>(height)));
}

vk::Extent2D talon::WindowManager::getWindowExtents() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    vk::Extent2D extent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };
    return extent;
}

