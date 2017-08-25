#pragma once
#include "TalonConfig.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "TreeComponentData.hpp"
#include "ComponentTransform.hpp"

TALON_NS_BEGIN

struct ComponentTransformTree : public ComponentTransform {
    TreeComponentData treeComponentData;

    ComponentTransformTree();
    ~ComponentTransformTree();
};

TALON_NS_END