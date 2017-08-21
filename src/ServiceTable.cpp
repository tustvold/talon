#include "ServiceTable.hpp"

USING_TALON_NS;

ServiceProvider<InstanceManager, nullptr_t> ServiceTable::instanceManager;
ServiceProvider<DeviceManager, nullptr_t> ServiceTable::deviceManager;
ServiceProvider<MemoryAllocator, nullptr_t> ServiceTable::memoryAllocator;
ServiceProvider<CommandPool, nullptr_t> ServiceTable::commandPool;