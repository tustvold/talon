#pragma once
#include "TalonConfig.hpp"
#include "WorldConfig.hpp"

TALON_NS_BEGIN

class GameSystem {
public:
    virtual ~GameSystem() = default;

    virtual void update(World &world) = 0;
};

TALON_NS_END