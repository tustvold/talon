#include <rendering/singleton/RenderServiceTable.hpp>
#include "MockApplication.hpp"
#include "MockCommandPool.hpp"
#include "MockDeviceManager.hpp"
#include "MockInstanceManager.hpp"
#include "MockMemoryAllocator.hpp"
#include "MockSurfaceManager.hpp"
#include "MockWindowManager.hpp"

USING_TALON_NS;

MockApplication::MockApplication() {
    mockCommandPool = std::make_unique<MockCommandPool>();
    RenderServiceTable::commandPool.set(mockCommandPool.get());
    mockDeviceManager = std::make_unique<MockDeviceManager>();
    RenderServiceTable::deviceManager.set(mockDeviceManager.get());
    mockInstanceManager = std::make_unique<MockInstanceManager>();
    RenderServiceTable::instanceManager.set(mockInstanceManager.get());
    mockMemoryAllocator  =std::make_unique<MockMemoryAllocator>();
    RenderServiceTable::memoryAllocator.set(mockMemoryAllocator.get());
    mockSurfaceManager = std::make_unique<MockSurfaceManager>();
    RenderServiceTable::surfaceManager.set(mockSurfaceManager.get());
    mockWindowManager = std::make_unique<MockWindowManager>();
    RenderServiceTable::windowManager.set(mockWindowManager.get());

}

MockApplication::~MockApplication() {

}
