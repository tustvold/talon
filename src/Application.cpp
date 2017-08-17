#include "talon.h"

#include "Application.hpp"
#include "vulkan_ext.hpp"
#include "Vertex.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <fstream>
#include <set>

const int WIDTH = 800;
const int HEIGHT = 600;

static const std::vector<const char *> validationLayers = {
    "VK_LAYER_LUNARG_standard_validation"
};

static std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

VkBool32 Application::vulkanDebugCallback(VkDebugReportFlagsEXT flags,
                                          VkDebugReportObjectTypeEXT objectType,
                                          uint64_t object,
                                          size_t location,
                                          int32_t messageCode,
                                          const char *pLayerPrefix,
                                          const char *pMessage,
                                          void *pUserData) {

    auto app = reinterpret_cast<Application*>(pUserData);
    return app->applicationDelegate->vulkanDebugCallback(flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

void Application::onWindowResized(GLFWwindow *window, int width, int height) {
    if (width == 0 || height == 0) return;

    auto *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    app->recreateSwapChain();
}


void Application::initVulkan() {
    createInstance();
    setupDebugCallback();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFrameBuffers();
    createCommandPool();
    createVertexBuffer();
    createCommandBuffers();
    createSemaphores();
}


void Application::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, Application::onWindowResized);
}

void Application::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device);
}

void Application::cleanupSwapChain() {
    for (auto &frameBuffer : swapChainFramebuffers) {
        device.destroyFramebuffer(frameBuffer, nullptr);
    }
    device.freeCommandBuffers(commandPool, commandBuffers);
    device.destroyPipeline(graphicsPipeline);
    device.destroyPipelineLayout(pipelineLayout);
    device.destroyRenderPass(renderPass);

    for (auto &imageView : swapChainImageViews) {
        device.destroyImageView(imageView);
    }
    device.destroySwapchainKHR(swapChain);
}

void Application::cleanup() {
    cleanupSwapChain();

    device.destroyBuffer(vertexBuffer);
    device.freeMemory(vertexBufferMemory);

    device.destroySemaphore(renderFinishedSemaphore);
    device.destroySemaphore(imageAvailableSemaphore);

    device.destroyCommandPool(commandPool);

    device.destroy();

    instance.destroyDebugReportCallbackEXT(callback);
    instance.destroySurfaceKHR(surface);
    instance.destroy();

    glfwDestroyWindow(window);
    glfwTerminate();
}

std::vector<char> Application::readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    auto fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

bool Application::checkValidationLayerSupport() {
    uint32_t layerCount;
    auto availableLayers = vk::enumerateInstanceLayerProperties();

    for (const char *layerName : validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

std::vector<const char *> Application::getRequiredExtensions() {
    std::vector<const char *> extensions;

    unsigned int glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}

void Application::recreateSwapChain() {
    device.waitIdle();

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFrameBuffers();
    createCommandBuffers();
}

void Application::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    vk::ApplicationInfo applicationInfo
        ("Hello Triangle", VK_MAKE_VERSION(1, 0, 0), "Talon", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0);

    vk::InstanceCreateInfo createInfo;
    createInfo.setPApplicationInfo(&applicationInfo);

    auto extensions = getRequiredExtensions();
    createInfo.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()));
    createInfo.setPpEnabledExtensionNames(extensions.data());

    if (enableValidationLayers) {
        createInfo.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()));
        createInfo.setPpEnabledLayerNames(validationLayers.data());
    } else {
        createInfo.setEnabledLayerCount(0);
    }
    instance = vk::createInstance(createInfo);
    vke::InitInstance(instance);
}


void Application::setupDebugCallback() {
    if (!enableValidationLayers) return;

    vk::DebugReportCallbackCreateInfoEXT createInfo;
    createInfo.setFlags(
        vk::Flags(vk::DebugReportFlagBitsEXT::eError) | vk::DebugReportFlagBitsEXT::eWarning);
    createInfo.setPfnCallback(vulkanDebugCallback);
    createInfo.pUserData = this;

    callback = instance.createDebugReportCallbackEXT(createInfo);
}

void Application::createSurface() {
    auto psurf = VkSurfaceKHR();
    if (glfwCreateWindowSurface(instance, window, nullptr, &psurf) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }

    surface = psurf;
}

void Application::pickPhysicalDevice() {
    uint32_t deviceCount = 0;

    auto devices = instance.enumeratePhysicalDevices();

    for (const auto &device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    TASSERT(physicalDevice);
}
void Application::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

    float queuePriority = 1.0f;
    for (auto queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.setQueueFamilyIndex((uint32_t) queueFamily);
        queueCreateInfo.setQueueCount(1);
        queueCreateInfo.setPQueuePriorities(&queuePriority);
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures;

    vk::DeviceCreateInfo createInfo = {};

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.setPEnabledFeatures(&deviceFeatures);

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }
    device = physicalDevice.createDevice(createInfo);
    graphicsQueue = device.getQueue(static_cast<uint32_t>(indices.graphicsFamily), 0);
    presentQueue = device.getQueue(static_cast<uint32_t>(indices.presentFamily), 0);
}

void Application::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

    auto surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    auto presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    auto extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0
        && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo = {};
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
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

    swapChain = device.createSwapchainKHR(createInfo);

    swapChainImages = device.getSwapchainImagesKHR(swapChain);
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}
void Application::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        vk::ImageViewCreateInfo createInfo = {};

        createInfo.image = swapChainImages[i];
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = vk::ComponentSwizzle::eIdentity;
        createInfo.components.g = vk::ComponentSwizzle::eIdentity;
        createInfo.components.b = vk::ComponentSwizzle::eIdentity;
        createInfo.components.a = vk::ComponentSwizzle::eIdentity;
        createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        swapChainImageViews[i] = device.createImageView(createInfo);
    }
}
void Application::createRenderPass() {
    vk::AttachmentDescription colorAttachment = {};
    colorAttachment.format = swapChainImageFormat;
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

    renderPass = device.createRenderPass(renderPassInfo);
}
void Application::createGraphicsPipeline() {
    auto vertShaderCode = readFile("shaders/shader.vert.spv");
    auto fragShaderCode = readFile("shaders/shader.frag.spv");

    auto vertShaderModule = createShaderModule(vertShaderCode);
    auto fragShaderModule = createShaderModule(fragShaderCode);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.setPVertexBindingDescriptions(&bindingDescription);
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    vk::Viewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vk::Rect2D scissor = {};
    scissor.offset = vk::Offset2D{0, 0};
    scissor.extent = swapChainExtent;

    vk::PipelineViewportStateCreateInfo viewportState = {};
    viewportState.viewportCount = 1;
    viewportState.setPViewports(&viewport);
    viewportState.scissorCount = 1;
    viewportState.setPScissors(&scissor);

    vk::PipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.depthBiasEnable = VK_FALSE;

    vk::PipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask =
        vk::Flags(vk::ColorComponentFlagBits::eR) | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB
            | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.setPAttachments(&colorBlendAttachment);
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo);

    vk::GraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.stageCount = 2;
    pipelineInfo.setPStages(shaderStages);
    pipelineInfo.setPVertexInputState(&vertexInputInfo);
    pipelineInfo.setPInputAssemblyState(&inputAssembly);
    pipelineInfo.setPViewportState(&viewportState);
    pipelineInfo.setPRasterizationState(&rasterizer);
    pipelineInfo.setPMultisampleState(&multisampling);
    pipelineInfo.setPColorBlendState(&colorBlending);
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = vk::Pipeline();

    graphicsPipeline = device.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);

    device.destroyShaderModule(fragShaderModule);
    device.destroyShaderModule(vertShaderModule);
}
void Application::createFrameBuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());

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

        swapChainFramebuffers[i] = device.createFramebuffer(framebufferInfo);
    }
}

void Application::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    vk::CommandPoolCreateInfo poolInfo = {};
    poolInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndices.graphicsFamily);

    commandPool = device.createCommandPool(poolInfo);
}

void Application::createVertexBuffer() {
    vk::BufferCreateInfo bufferInfo = {};
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    vertexBuffer = device.createBuffer(bufferInfo);

    vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements(vertexBuffer);

    vk::MemoryAllocateInfo allocInfo = {};
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                               vk::Flags(vk::MemoryPropertyFlagBits::eHostVisible)
                                                   | vk::MemoryPropertyFlagBits::eHostCoherent);
    vertexBufferMemory = device.allocateMemory(allocInfo);

    device.bindBufferMemory(vertexBuffer, vertexBufferMemory, 0);

    void *data;
    device.mapMemory(vertexBufferMemory, 0, bufferInfo.size, vk::MemoryMapFlags(), &data);
    memcpy(data, vertices.data(), (size_t) bufferInfo.size);
    device.unmapMemory(vertexBufferMemory);
}

uint32_t Application::findMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags &properties) {
    auto memProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type at "  S_LOCATION);
}

void Application::createCommandBuffers() {
    commandBuffers.resize(swapChainFramebuffers.size());

    vk::CommandBufferAllocateInfo allocInfo = {};
    allocInfo.commandPool = commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    commandBuffers = device.allocateCommandBuffers(allocInfo);

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        vk::CommandBufferBeginInfo beginInfo = {};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

        commandBuffers[i].begin(beginInfo);

        vk::RenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
        renderPassInfo.renderArea.extent = swapChainExtent;

        vk::ClearValue clearColor;
        clearColor.setColor(vk::ClearColorValue());
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.setPClearValues(&clearColor);

        commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

        commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

        vk::Buffer vertexBuffers[] = {vertexBuffer};
        vk::DeviceSize offsets[] = {0};

        commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffers, offsets);

        commandBuffers[i].draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0);

        commandBuffers[i].endRenderPass();
        commandBuffers[i].end();
    }
}

void Application::createSemaphores() {
    vk::SemaphoreCreateInfo semaphoreInfo = {};

    imageAvailableSemaphore = device.createSemaphore(semaphoreInfo);
    renderFinishedSemaphore = device.createSemaphore(semaphoreInfo);
}

void Application::drawFrame() {
    uint32_t imageIndex;

    vk::Result result = device.acquireNextImageKHR(swapChain,
                                                   std::numeric_limits<uint64_t>::max(),
                                                   imageAvailableSemaphore,
                                                   vk::Fence(),
                                                   &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR) {
        recreateSwapChain();
        return;
    } else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vk::SubmitInfo submitInfo = {};

    vk::Semaphore waitSemaphores[] = {imageAvailableSemaphore};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;

    submitInfo.setPWaitSemaphores(waitSemaphores);
    submitInfo.setPWaitDstStageMask(waitStages);

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    vk::Semaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.setPSignalSemaphores(signalSemaphores);

    graphicsQueue.submit(1, &submitInfo, vk::Fence());

    vk::PresentInfoKHR presentInfo = {};

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.setPWaitSemaphores(signalSemaphores);

    vk::SwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.setPSwapchains(swapChains);

    presentInfo.setPImageIndices(&imageIndex);

    result = presentQueue.presentKHR(presentInfo);

    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
        recreateSwapChain();
    } else if (result != vk::Result::eSuccess) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    vkQueueWaitIdle(presentQueue);
}

vk::ShaderModule Application::createShaderModule(const std::vector<char> &code) {
    vk::ShaderModuleCreateInfo createInfo = {};
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    return device.createShaderModule(createInfo);
}

vk::SurfaceFormatKHR Application::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
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

vk::PresentModeKHR Application::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
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

vk::Extent2D Application::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        vk::Extent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width,
                                      std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
                                       std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

SwapChainSupportDetails Application::querySwapChainSupport(vk::PhysicalDevice device) {
    SwapChainSupportDetails details;

    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);

    return details;
}

bool Application::isDeviceSuitable(vk::PhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool Application::checkDeviceExtensionSupport(vk::PhysicalDevice device) {
    auto availableExtensions = device.enumerateDeviceExtensionProperties();
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

QueueFamilyIndices Application::findQueueFamilies(vk::PhysicalDevice device) {
    QueueFamilyIndices indices;

    auto queueFamilies = device.getQueueFamilyProperties();

    uint32_t i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        vk::Bool32 presentSupport = VK_FALSE;
        device.getSurfaceSupportKHR(i, surface, &presentSupport);

        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}
