#pragma once
#include "TalonConfig.hpp"
#include "vulkan/vulkan.hpp"

TALON_NS_BEGIN

class VSemaphore {
public:
    VSemaphore();
    ~VSemaphore();

    vk::Semaphore get() const {
        return semaphore;
    }

private:
    vk::Semaphore semaphore;
};


TALON_NS_END