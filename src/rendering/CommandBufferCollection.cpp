#include "CommandBufferCollection.hpp"
#include "ApplicationServiceTable.hpp"
#include "CommandPool.hpp"

talon::CommandBufferCollection::CommandBufferCollection(uint32_t size, vk::CommandBufferLevel level) {
    collection = ApplicationServiceTable::commandPool->createCommandBuffers(size, level);
}
talon::CommandBufferCollection::~CommandBufferCollection() {
    ApplicationServiceTable::commandPool->destroyCommandBuffers(collection);
}
