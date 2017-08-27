#pragma once
#include "TalonConfig.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"
#include <ecs/annotations/Annotations.hpp>
#include <ecs/component/ComponentModelMatrix.hpp>

TALON_NS_BEGIN

struct ComponentTransform {
    ADD_DEPENDENCY_ANNOTATION(ComponentModelMatrix)

    Eigen::Vector3f position;
    Eigen::Vector3f scale;
    Eigen::Quaternionf rotation;
    bool dirty;

    ComponentTransform();
    ~ComponentTransform();

    void updateTransform(Eigen::Matrix4f& transform) const;
};

TALON_NS_END