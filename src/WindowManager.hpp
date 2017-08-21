#pragma once
#include <Signal.hpp>
#include "TalonConfig.hpp"
#include "InstanceManager.hpp"
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

    vk::Extent2D getWindowExtents();

    bool poll();

    auto& getWindowResizeEvent() {
        return windowResizeEvent;
    }

private:
    GLFWwindow* window;

    Gallant::Signal2<GLFWwindow*, vk::Extent2D> windowResizeEvent;

    static void onWindowResizedStatic(GLFWwindow *window, int width, int height);
    void onWindowResized(GLFWwindow *window, int width, int height);
};

TALON_NS_END



