#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class InstanceManager;
class WindowManager;

class SurfaceManager {
public:
    SurfaceManager(InstanceManager* instanceManager, WindowManager* windowManager);

    vk::SurfaceKHR getSurface() const {
        return surface;
    }

private:
    vk::SurfaceKHR surface;
};


TALON_NS_END



