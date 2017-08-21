#include "RenderPass.hpp"
#include "ServiceTable.hpp"
#include "SwapChain.hpp"
#include "DeviceManager.hpp"

USING_TALON_NS;

RenderPass::RenderPass(const SwapChain *swapChain,
                              DeviceManager* deviceManager) {
    createRenderPass(swapChain, deviceManager);
    createFrameBuffers(swapChain, deviceManager);
}

RenderPass::~RenderPass() {
    for (auto &frameBuffer : framebuffers) {
        ServiceTable::deviceManager->getDevice().destroyFramebuffer(frameBuffer);
    }
    ServiceTable::deviceManager->getDevice().destroyRenderPass(renderPass);
}

void RenderPass::createRenderPass(const SwapChain *swapChain, DeviceManager *deviceManager) {
    vk::AttachmentDescription colorAttachment = {};
    colorAttachment.format = swapChain->getImageFormat();
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass = {};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.setPColorAttachments(&colorAttachmentRef);

    vk::SubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.srcAccessMask = vk::AccessFlags(); // TODO: Not sure about this
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstAccessMask = vk::Flags(vk::AccessFlagBits::eColorAttachmentRead) |
        vk::AccessFlagBits::eColorAttachmentWrite;

    vk::RenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.setPAttachments(&colorAttachment);
    renderPassInfo.subpassCount = 1;
    renderPassInfo.setPSubpasses(&subpass);
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.setPDependencies(&dependency);

    renderPass = deviceManager->getDevice().createRenderPass(renderPassInfo);
}

void RenderPass::createFrameBuffers(const SwapChain *swapChain, DeviceManager *deviceManager) {
    auto &swapChainImageViews = swapChain->getImageViews();
    auto swapChainExtent = swapChain->getExtents();

    framebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        vk::ImageView attachments[] = {
            swapChainImageViews[i]
        };

        vk::FramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.setPAttachments(attachments);
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        framebuffers[i] = deviceManager->getDevice().createFramebuffer(framebufferInfo);
    }
}
