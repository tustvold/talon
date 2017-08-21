#include "VSemaphore.hpp"
#include "ServiceTable.hpp"

talon::VSemaphore::VSemaphore() {
    vk::SemaphoreCreateInfo semaphoreInfo = {};
    semaphore = ServiceTable::deviceProvider->getDevice().createSemaphore(semaphoreInfo);
}

talon::VSemaphore::~VSemaphore() {
    ServiceTable::deviceProvider->getDevice().destroySemaphore(semaphore);
}
