#pragma once
#include <TalonConfig.hpp>
#include <memory>

TALON_NS_BEGIN

class MockCommandPool;
class MockDeviceManager;
class MockInstanceManager;
class MockMemoryAllocator;
class MockSurfaceManager;
class MockWindowManager;

class MockApplication {
public:
    MockApplication();
    ~MockApplication();

    MockCommandPool* getCommandPool() {
        return mockCommandPool.get();
    }

    MockDeviceManager* getDeviceManager() {
        return mockDeviceManager.get();
    }

    MockInstanceManager* getInstanceManager() {
        return mockInstanceManager.get();
    }

    MockMemoryAllocator* getMemoryAllocator() {
        return mockMemoryAllocator.get();
    }

    MockSurfaceManager* getSurfaceManager() {
        return mockSurfaceManager.get();
    }

    MockWindowManager* getWindowManager() {
        return mockWindowManager.get();
    }

private:
    std::unique_ptr<MockCommandPool> mockCommandPool;
    std::unique_ptr<MockDeviceManager> mockDeviceManager;
    std::unique_ptr<MockInstanceManager> mockInstanceManager;
    std::unique_ptr<MockMemoryAllocator> mockMemoryAllocator;
    std::unique_ptr<MockSurfaceManager> mockSurfaceManager;
    std::unique_ptr<MockWindowManager> mockWindowManager;

};



TALON_NS_END