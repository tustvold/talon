#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <rendering/singleton/InstanceManager.hpp>

TALON_NS_BEGIN

class MockInstanceManager : public InstanceManager {
public:
    MOCK_CONST_METHOD1(createDebugReportCallbackEXT, vk::DebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT &createInfoEXT));
    MOCK_CONST_METHOD1(destroyDebugReportCallbackEXT, void(vk::DebugReportCallbackEXT callbackEXT));
};

TALON_NS_END