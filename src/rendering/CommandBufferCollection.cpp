#include "CommandBufferCollection.hpp"
#include "ServiceTable.hpp"
#include "CommandPool.hpp"

talon::CommandBufferCollection::CommandBufferCollection(uint32_t size, vk::CommandBufferLevel level) {
    collection = ServiceTable::commandPool->createCommandBuffers(size, level);
}
talon::CommandBufferCollection::~CommandBufferCollection() {
    ServiceTable::commandPool->destroyCommandBuffers(collection);
}
