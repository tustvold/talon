#pragma once
#include <vulkan/vulkan.hpp>
#include "application/ApplicationInitSettings.hpp"
#include "TalonConfig.hpp"
#include "Signal.hpp"

struct GLFWwindow;


TALON_NS_BEGIN

class WindowManager {
public:
    explicit WindowManager(const ApplicationInitSettings& initSettings);

    ~WindowManager();

    GLFWwindow* getWindow() {
        return window;
    }

    vk::Extent2D getWindowExtents() const;

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



