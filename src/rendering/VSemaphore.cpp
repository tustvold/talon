#include "VSemaphore.hpp"
#include "ServiceTable.hpp"
#include "DeviceManager.hpp"

USING_TALON_NS;

VSemaphore::VSemaphore() {
    vk::SemaphoreCreateInfo semaphoreInfo = {};
    semaphore = ServiceTable::deviceManager->getDevice().createSemaphore(semaphoreInfo);
}

VSemaphore::~VSemaphore() {
    ServiceTable::deviceManager->getDevice().destroySemaphore(semaphore);
}
