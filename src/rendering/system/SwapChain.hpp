#pragma once
#include "TalonConfig.hpp"
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class WindowManager;
class SurfaceManager;
class DeviceManager;
class RenderPass;

class SwapChain {
public:
    explicit SwapChain(int numRenderPasses);
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

    RenderPass* getRenderPass(size_t index);

private:
    vk::SwapchainKHR swapChain;
    std::vector<vk::Image> images;
    vk::Format imageFormat;
    vk::Extent2D extents;
    std::vector<vk::ImageView> imageViews;
    std::vector<RenderPass> renderPasses;

    void createSwapChain();
    void createImageViews();

    static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);
    static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);
    static vk::Extent2D chooseSwapExtent(const WindowManager *windowManager, const vk::SurfaceCapabilitiesKHR &capabilities);
};

TALON_NS_END





