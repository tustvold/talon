#include "ServiceTable.hpp"

USING_TALON_NS;

ServiceProvider<DeviceManager, nullptr_t> ServiceTable::deviceProvider;
ServiceProvider<MemoryAllocator, nullptr_t> ServiceTable::memoryAllocator;
ServiceProvider<CommandPool, nullptr_t> ServiceTable::commandPool;