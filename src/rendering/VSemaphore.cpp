#include "VSemaphore.hpp"
#include "ApplicationServiceTable.hpp"
#include "DeviceManager.hpp"

USING_TALON_NS;

VSemaphore::VSemaphore() {
    vk::SemaphoreCreateInfo semaphoreInfo = {};
    semaphore = ApplicationServiceTable::deviceManager->getDevice().createSemaphore(semaphoreInfo);
}

VSemaphore::~VSemaphore() {
    ApplicationServiceTable::deviceManager->getDevice().destroySemaphore(semaphore);
}
