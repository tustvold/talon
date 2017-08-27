#pragma once
#include <ecs/annotations/Annotations.hpp>
#include "TalonConfig.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "ecs/annotations/AnnotationTree.hpp"
#include "ComponentTransform.hpp"

TALON_NS_BEGIN

struct ComponentTransformTree : public ComponentTransform {
    ADD_DEPENDENCY_ANNOTATION(ComponentModelMatrix)
    ADD_TREE_ANNOTATION();

    ComponentTransformTree();
    ~ComponentTransformTree();
};

TALON_NS_END