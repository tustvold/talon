#pragma once
#include <rendering/material/TDescriptorPool.hpp>
#include <ecs/component/ComponentModelMatrix.hpp>

USING_TALON_NS;

using DescriptorSetModelMatrix = DescriptorSetLayout<1, ComponentModelMatrix>;

using DescriptorPool = TDescriptorPool<DescriptorSetModelMatrix>;