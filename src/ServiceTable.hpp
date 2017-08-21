#pragma once
#include "TalonConfig.hpp"
#include "ServiceProvider.hpp"

TALON_NS_BEGIN
class InstanceManager;
class DeviceManager;
class MemoryAllocator;
class CommandPool;

struct ServiceTable {
public:
    ServiceTable() = delete;
    ~ServiceTable() = delete;

    static ServiceProvider<InstanceManager, nullptr_t> instanceManager;
    static ServiceProvider<DeviceManager, nullptr_t> deviceManager;
    static ServiceProvider<MemoryAllocator, nullptr_t> memoryAllocator;
    static ServiceProvider<CommandPool, nullptr_t> commandPool;
};

TALON_NS_END