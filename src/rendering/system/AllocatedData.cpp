#include <util/Logging.hpp>
#include <rendering/singleton/RenderServiceTable.hpp>
#include <rendering/singleton/MemoryAllocator.hpp>
#include "AllocatedData.hpp"

USING_TALON_NS;

void AllocatedDataMapHandleBase::mapMemory(VmaAllocation allocation, void **ptr) {
    auto result = RenderServiceTable::memoryAllocator->mapMemory(allocation, ptr);
    TASSERT(result == vk::Result::eSuccess);
}
void AllocatedDataMapHandleBase::unmapMemory(VmaAllocation allocation) {

}