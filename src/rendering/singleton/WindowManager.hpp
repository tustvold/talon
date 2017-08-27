#pragma once
#include <TalonConfig.hpp>


TALON_NS_BEGIN
class WindowManager {
public:
    virtual vk::Extent2D getWindowExtents() const = 0;
};
TALON_NS_END