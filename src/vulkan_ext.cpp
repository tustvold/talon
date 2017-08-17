#include <talon.h>
#include "vulkan_ext.hpp"

static PFN_vkCreateDebugReportCallbackEXT __vkCreateDebugReportCallbackEXT;
static PFN_vkDestroyDebugReportCallbackEXT __vkDestroyDebugReportCallbackEXT;

void ::vke::InitInstance(vk::Instance instance) {
    __vkCreateDebugReportCallbackEXT =
        (PFN_vkCreateDebugReportCallbackEXT) instance.getProcAddr("vkCreateDebugReportCallbackEXT");
    __vkDestroyDebugReportCallbackEXT =
        (PFN_vkDestroyDebugReportCallbackEXT) instance.getProcAddr("vkDestroyDebugReportCallbackEXT");

    TASSERT(__vkCreateDebugReportCallbackEXT);
    TASSERT(__vkDestroyDebugReportCallbackEXT);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(
    VkInstance instance,
    const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugReportCallbackEXT *pCallback) {
    __vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(
    VkInstance instance,
    VkDebugReportCallbackEXT callback,
    const VkAllocationCallbacks *pAllocator) {
    __vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}