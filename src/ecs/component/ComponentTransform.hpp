#pragma once
#include "TalonConfig.hpp"
#include "util/Logging.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"

TALON_NS_BEGIN

struct ComponentTransform {
    Eigen::Vector3f position;
    Eigen::Vector3f scale;
    Eigen::Quaternionf rotation;
    bool localTransformDirty;

    Eigen::Transform<float, 3, Eigen::Affine> localTransform;
    Eigen::Transform<float, 3, Eigen::Affine> worldTransform;

    ComponentTransform() : position(0,0,0), scale(1,1,1), rotation(1,0,0,0), localTransformDirty(true) {

    }

    void updateLocalTransform() {
        if (!localTransformDirty)
            return;
        localTransformDirty = false;
        localTransform = Eigen::Translation3f(position) * rotation.toRotationMatrix() * Eigen::Scaling(scale);
    }

    void updateWorldTransform(ComponentTransform* parent) {
        TASSERT(!parent->localTransformDirty);
        TASSERT(!localTransformDirty);
        worldTransform = parent->worldTransform * localTransform;
    }
};

TALON_NS_END