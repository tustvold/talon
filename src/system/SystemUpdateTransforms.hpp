#pragma once
#include "TalonConfig.hpp"
#include "WorldConfig.hpp"

TALON_NS_BEGIN

class SystemUpdateTransforms {
public:
    void update(World &world);
};

TALON_NS_END