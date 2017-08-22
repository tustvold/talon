#pragma once
#include "TalonConfig.hpp"
#include <WorldConfig.hpp>

TALON_NS_BEGIN

class SystemRenderMeshFilter {
public:
    void update(World& world, vk::CommandBuffer commandBuffer);
};


TALON_NS_END