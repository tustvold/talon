#include "DebugCallback.hpp"
#include "ApplicationServiceTable.hpp"
#include "InstanceManager.hpp"

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

    callback = ApplicationServiceTable::instanceManager->getInstance().createDebugReportCallbackEXT(createInfo);
}

DebugCallback::~DebugCallback() {
    ApplicationServiceTable::instanceManager->getInstance().destroyDebugReportCallbackEXT(callback);
}

