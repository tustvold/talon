#pragma once

#include "talon.hpp"
#include "ServiceProvider.hpp"
#include "DeviceProvider.hpp"

TALON_NS_BEGIN

struct SystemTable {
public:
    SystemTable() = delete;
    ~SystemTable() = delete;

    static ServiceProvider<DeviceProvider, NullDeviceProvider> deviceProvider;
};

TALON_NS_END