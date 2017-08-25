#pragma once
#include "TalonConfig.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"

TALON_NS_BEGIN

struct ComponentTransform {
    Eigen::Vector3f position;
    Eigen::Vector3f scale;
    Eigen::Quaternionf rotation;
    bool dirty;

    ComponentTransform();
    ~ComponentTransform();

    void updateTransform(Eigen::Matrix4f& transform) const;
};

TALON_NS_END