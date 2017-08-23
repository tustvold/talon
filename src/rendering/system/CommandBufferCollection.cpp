#include "CommandBufferCollection.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/singleton/CommandPool.hpp"

talon::CommandBufferCollection::CommandBufferCollection(uint32_t size, vk::CommandBufferLevel level) {
    collection = RenderServiceTable::commandPool->createCommandBuffers(size, level);
}
talon::CommandBufferCollection::~CommandBufferCollection() {
    RenderServiceTable::commandPool->destroyCommandBuffers(collection);
}
