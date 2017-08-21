#pragma once

#include "TalonConfig.hpp"
#include "ServiceProvider.hpp"
#include "DeviceManager.hpp"
#include "MemoryAllocator.hpp"
#include "CommandPool.hpp"

TALON_NS_BEGIN

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