#include "ServiceTable.hpp"

USING_TALON_NS;

ServiceProvider<DeviceProvider, NullDeviceProvider> ServiceTable::deviceProvider;