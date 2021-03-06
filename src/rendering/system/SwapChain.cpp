#include "SwapChain.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/singleton/impl/VulkanWindowManager.hpp"
#include "rendering/singleton/impl/VulkanSurfaceManager.hpp"
#include "rendering/singleton/impl/VulkanDeviceManager.hpp"
#include "RenderPass.hpp"

USING_TALON_NS;

vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
    if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
        return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    }

    for (const auto &availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Unorm
            && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
    auto bestMode = vk::PresentModeKHR::eMailbox;

    for (const auto &availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        } else if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
            bestMode = availablePresentMode;
        }
    }

    return bestMode;
}

vk::Extent2D SwapChain::chooseSwapExtent(const VulkanWindowManager *windowManager, const vk::SurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        auto actualExtent = windowManager->getWindowExtents();

        actualExtent.width = std::max(capabilities.minImageExtent.width,
                                      std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
                                       std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

SwapChain::SwapChain(VulkanDeviceManager* deviceManager, VulkanSurfaceManager* surfaceManager, VulkanWindowManager* windowManager, int numberRenderPasses)
    : imageFormat(vk::Format::eUndefined) {
    createSwapChain(deviceManager, surfaceManager, windowManager);
    createImageViews();

    for(int i = 0; i < numberRenderPasses; i++)
        renderPasses.emplace_back(std::make_unique<RenderPass>(this));
}

SwapChain::~SwapChain() {
    for (auto &imageView : imageViews) {
        RenderServiceTable::deviceManager->destroyImageView(imageView);
    }
    RenderServiceTable::deviceManager->destroySwapchainKHR(swapChain);
}

void SwapChain::createSwapChain(VulkanDeviceManager* deviceManager, VulkanSurfaceManager* surfaceManager, VulkanWindowManager* windowManager) {
    SwapChainSupportDetails swapChainSupport = deviceManager->getSwapChainSupportDetails(surfaceManager);

    auto surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    auto presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    auto extent = chooseSwapExtent(windowManager, swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0
        && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo = {};
    createInfo.surface = surfaceManager->getSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    QueueFamilyIndices indices = deviceManager->getQueueFamilyIndices(surfaceManager);
    uint32_t queueFamilyIndices[] = {(uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.setPQueueFamilyIndices(queueFamilyIndices);
    } else {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    swapChain = deviceManager->createSwapchainKHR(createInfo);

    images = deviceManager->getSwapchainImagesKHR(swapChain);
    imageFormat = surfaceFormat.format;
    extents = extent;
}

void SwapChain::createImageViews() {
    imageViews.resize(images.size());

    for (size_t i = 0; i < images.size(); i++) {
        vk::ImageViewCreateInfo createInfo = {};

        createInfo.image = images[i];
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.format = imageFormat;
        createInfo.components.r = vk::ComponentSwizzle::eIdentity;
        createInfo.components.g = vk::ComponentSwizzle::eIdentity;
        createInfo.components.b = vk::ComponentSwizzle::eIdentity;
        createInfo.components.a = vk::ComponentSwizzle::eIdentity;
        createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        imageViews[i] = RenderServiceTable::deviceManager->createImageView(createInfo);
    }
}
RenderPass *SwapChain::getRenderPass(size_t index) {
    return renderPasses[index].get();
}
