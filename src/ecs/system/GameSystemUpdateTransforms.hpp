#pragma once
#include "TalonConfig.hpp"
#include "WorldConfig.hpp"
#include "GameSystem.hpp"

TALON_NS_BEGIN

class GameSystemUpdateTransforms : public GameSystem {
public:
    void update(World &world) override;
};

TALON_NS_END