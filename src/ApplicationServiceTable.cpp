#include "ApplicationServiceTable.hpp"

USING_TALON_NS;

ServiceProvider<InstanceManager, nullptr_t> ApplicationServiceTable::instanceManager;
ServiceProvider<DeviceManager, nullptr_t> ApplicationServiceTable::deviceManager;
ServiceProvider<MemoryAllocator, nullptr_t> ApplicationServiceTable::memoryAllocator;
ServiceProvider<CommandPool, nullptr_t> ApplicationServiceTable::commandPool;