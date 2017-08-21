#include "VSemaphore.hpp"
#include "ServiceTable.hpp"

talon::VSemaphore::VSemaphore() {
    vk::SemaphoreCreateInfo semaphoreInfo = {};
    semaphore = ServiceTable::deviceManager->getDevice().createSemaphore(semaphoreInfo);
}

talon::VSemaphore::~VSemaphore() {
    ServiceTable::deviceManager->getDevice().destroySemaphore(semaphore);
}
