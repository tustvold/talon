#pragma once

#include "TalonConfig.hpp"
#include "ServiceProvider.hpp"
#include "DeviceProvider.hpp"

TALON_NS_BEGIN

struct ServiceTable {
public:
    ServiceTable() = delete;
    ~ServiceTable() = delete;

    static ServiceProvider<DeviceProvider, NullDeviceProvider> deviceProvider;
};

TALON_NS_END