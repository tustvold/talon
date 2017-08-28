#include "RenderPass.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/system/SwapChain.hpp"
#include "rendering/singleton/DeviceManager.hpp"
#include "rendering/material/MaterialPipelineCache.hpp"

USING_TALON_NS;

RenderPass::RenderPass(SwapChain *swapChain) {
    materialPipelineCache = std::make_unique<MaterialPipelineCache>();
    createRenderPass(swapChain);
    createFrameBuffers(swapChain);
}

RenderPass::~RenderPass() {
    for (auto &frameBuffer : framebuffers) {
        RenderServiceTable::deviceManager->destroyFramebuffer(frameBuffer);
    }
    RenderServiceTable::deviceManager->destroyRenderPass(renderPass);
}

void RenderPass::createRenderPass(SwapChain* swapChain) {
    auto deviceManager = RenderServiceTable::deviceManager;
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

    renderPass = deviceManager->createRenderPass(renderPassInfo);
}

void RenderPass::createFrameBuffers(SwapChain* swapChain) {
    auto deviceManager = RenderServiceTable::deviceManager;
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

        framebuffers[i] = deviceManager->createFramebuffer(framebufferInfo);
    }
}

void RenderPass::bindMaterial(SwapChain* swapChain, talon::Material *material, CommandBuffer* buffer) {
    materialPipelineCache->bindMaterial(material, swapChain, this, buffer);
}
