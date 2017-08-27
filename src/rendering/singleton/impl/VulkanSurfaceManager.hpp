#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>
#include "rendering/singleton/SurfaceManager.hpp"

TALON_NS_BEGIN

class VulkanInstanceManager;
class VulkanWindowManager;

class VulkanSurfaceManager : public SurfaceManager {
public:
    VulkanSurfaceManager(VulkanInstanceManager* instanceManager, VulkanWindowManager* windowManager);
    ~VulkanSurfaceManager();

    vk::SurfaceKHR getSurface() const {
        return surface;
    }

private:
    vk::SurfaceKHR surface;
};


TALON_NS_END



