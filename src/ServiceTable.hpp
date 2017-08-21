#pragma once

#include "TalonConfig.hpp"
#include "ServiceProvider.hpp"
#include "DeviceProvider.hpp"
#include "MemoryAllocator.hpp"
#include "CommandPool.hpp"

TALON_NS_BEGIN

struct ServiceTable {
public:
    ServiceTable() = delete;
    ~ServiceTable() = delete;

    static ServiceProvider<DeviceProvider, nullptr_t> deviceProvider;
    static ServiceProvider<MemoryAllocator, nullptr_t> memoryAllocator;
    static ServiceProvider<CommandPool, nullptr_t> commandPool;
};

TALON_NS_END