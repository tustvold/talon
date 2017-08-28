#include "RenderServiceTable.hpp"

USING_TALON_NS;

ServiceProvider<InstanceManager, nullptr_t> RenderServiceTable::instanceManager;
ServiceProvider<DeviceManager, nullptr_t> RenderServiceTable::deviceManager;
ServiceProvider<MemoryAllocator, nullptr_t> RenderServiceTable::memoryAllocator;
ServiceProvider<CommandPool, nullptr_t> RenderServiceTable::commandPool;
ServiceProvider<SurfaceManager, nullptr_t> RenderServiceTable::surfaceManager;
ServiceProvider<WindowManager, nullptr_t> RenderServiceTable::windowManager;
ServiceProvider<DescriptorManager, nullptr_t> RenderServiceTable::descriptorManager;