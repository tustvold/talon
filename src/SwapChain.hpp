#pragma once
#include "talon.hpp"
#include "DeviceManager.hpp"
#include "WindowManager.hpp"
#include "SurfaceManager.hpp"

TALON_NS_BEGIN

class SwapChain {
public:
    SwapChain(WindowManager* windowManager, SurfaceManager* surfaceManager, const std::shared_ptr<DeviceManager>& deviceManager);
    ~SwapChain();

    const vk::SwapchainKHR &getSwapChain() const {
        return swapChain;
    }

    const std::vector<vk::Image> &getSwapChainImages() const {
        return images;
    }

    vk::Format getImageFormat() const {
        return imageFormat;
    }

    const vk::Extent2D &getExtents() const {
        return extents;
    }

    const std::vector<vk::ImageView> &getImageViews() const {
        return imageViews;
    }

private:
    vk::SwapchainKHR swapChain;
    std::vector<vk::Image> images;
    vk::Format imageFormat;
    vk::Extent2D extents;
    std::vector<vk::ImageView> imageViews;

    std::shared_ptr<DeviceManager> deviceManager;

    void createSwapChain(WindowManager* windowManager, SurfaceManager* surfaceManager);
    void createImageViews();

    static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);
    static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);
    static vk::Extent2D chooseSwapExtent(WindowManager *windowManager, const vk::SurfaceCapabilitiesKHR &capabilities);
};

TALON_NS_END





