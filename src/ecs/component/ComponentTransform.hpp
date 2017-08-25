#pragma once
#include "TalonConfig.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "TreeComponent.hpp"

TALON_NS_BEGIN

struct ComponentTransform : public TreeComponent {
    Eigen::Vector3f position;
    Eigen::Vector3f scale;
    Eigen::Quaternionf rotation;
    bool localTransformDirty;

    Eigen::Matrix4f localTransform;
    Eigen::Matrix4f worldTransform;

    ComponentTransform();
    ~ComponentTransform();

    void updateLocalTransform();

    void updateWorldTransform(ComponentTransform* parent);
};

TALON_NS_END