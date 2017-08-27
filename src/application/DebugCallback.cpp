#include "DebugCallback.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/singleton/impl/VulkanInstanceManager.hpp"

USING_TALON_NS;

VDebugCallbackArgs::VDebugCallbackArgs(VkDebugReportFlagsEXT flags,
                                       VkDebugReportObjectTypeEXT objectType,
                                       uint64_t object,
                                       size_t location,
                                       int32_t messageCode,
                                       const char *pLayerPrefix,
                                       const char *pMessage)
    : flags(flags),
      objectType(objectType),
      object(object),
      location(location),
      messageCode(messageCode),
      pLayerPrefix(pLayerPrefix),
      pMessage(pMessage) {}

VkBool32 DebugCallback::vulkanDebugCallback(VkDebugReportFlagsEXT flags,
                                          VkDebugReportObjectTypeEXT objectType,
                                          uint64_t object,
                                          size_t location,
                                          int32_t messageCode,
                                          const char *pLayerPrefix,
                                          const char *pMessage,
                                          void *pUserData) {

    VDebugCallbackArgs args(flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);

    auto debugCallback = reinterpret_cast<DebugCallback *>(pUserData);
    debugCallback->delegate(args);
    return VK_FALSE;
}

DebugCallback::DebugCallback(const Gallant::Delegate1<const VDebugCallbackArgs&> &delegate) : delegate(delegate) {
    vk::DebugReportCallbackCreateInfoEXT createInfo;
    createInfo.setFlags(
        vk::Flags(vk::DebugReportFlagBitsEXT::eError) | vk::DebugReportFlagBitsEXT::eWarning);
    createInfo.setPfnCallback(vulkanDebugCallback);
    createInfo.pUserData = this;

    callback = RenderServiceTable::instanceManager->createDebugReportCallbackEXT(createInfo);
}

DebugCallback::~DebugCallback() {
    RenderServiceTable::instanceManager->destroyDebugReportCallbackEXT(callback);
}

