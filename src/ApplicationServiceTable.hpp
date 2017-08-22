#pragma once
#include "TalonConfig.hpp"
#include "ServiceProvider.hpp"

TALON_NS_BEGIN
class InstanceManager;
class DeviceManager;
class SurfaceManager;
class WindowManager;
class MemoryAllocator;
class CommandPool;

struct ApplicationServiceTable {
public:
    ApplicationServiceTable() = delete;
    ~ApplicationServiceTable() = delete;

    static ServiceProvider<InstanceManager, nullptr_t> instanceManager;
    static ServiceProvider<DeviceManager, nullptr_t> deviceManager;
    static ServiceProvider<SurfaceManager, nullptr_t> surfaceManager;
    static ServiceProvider<WindowManager, nullptr_t> windowManager;
    static ServiceProvider<MemoryAllocator, nullptr_t> memoryAllocator;
    static ServiceProvider<CommandPool, nullptr_t> commandPool;
};

TALON_NS_END