#pragma once
#include <vulkan/vulkan.hpp>
#include "application/ApplicationInitSettings.hpp"
#include "rendering/singleton/WindowManager.hpp"
#include "TalonConfig.hpp"
#include "Signal.hpp"

struct GLFWwindow;


TALON_NS_BEGIN

class VulkanWindowManager : public WindowManager {
public:
    explicit VulkanWindowManager(const ApplicationInitSettings& initSettings);

    ~VulkanWindowManager();

    GLFWwindow* getWindow() {
        return window;
    }

    vk::Extent2D getWindowExtents() const final;

    bool poll();

    auto& getWindowResizeEvent() {
        return windowResizeEvent;
    }

private:
    GLFWwindow* window;

    Gallant::Signal1<vk::Extent2D> windowResizeEvent;

    static void onWindowResizedStatic(GLFWwindow *window, int width, int height);
    void onWindowResized(GLFWwindow *window, int width, int height);
};

TALON_NS_END



