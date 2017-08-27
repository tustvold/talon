#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <rendering/singleton/WindowManager.hpp>
#include <vulkan/vulkan.hpp>

TALON_NS_BEGIN

class MockWindowManager : public WindowManager {
public:
    MOCK_CONST_METHOD0(getWindowExtents, vk::Extent2D());
};


TALON_NS_END