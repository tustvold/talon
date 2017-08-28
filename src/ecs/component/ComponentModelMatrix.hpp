#pragma once
#include <TalonConfig.hpp>
#include "Eigen/Core"

TALON_NS_BEGIN

struct ComponentModelMatrix {
    Eigen::Matrix4f modelMatrix;
};

TALON_NS_END