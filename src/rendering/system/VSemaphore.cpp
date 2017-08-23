#include "VSemaphore.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/singleton/DeviceManager.hpp"

USING_TALON_NS;

VSemaphore::VSemaphore() {
    vk::SemaphoreCreateInfo semaphoreInfo = {};
    semaphore = RenderServiceTable::deviceManager->getDevice().createSemaphore(semaphoreInfo);
}

VSemaphore::~VSemaphore() {
    RenderServiceTable::deviceManager->getDevice().destroySemaphore(semaphore);
}
