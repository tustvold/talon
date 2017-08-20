#pragma once
#include "Talon.hpp"
#include "WindowManager.hpp"

TALON_NS_BEGIN

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



