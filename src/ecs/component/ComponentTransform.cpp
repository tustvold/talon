#include "ComponentTransform.hpp"
#include "util/Logging.hpp"

USING_TALON_NS;

ComponentTransform::ComponentTransform() : position(0,0,0), scale(1,1,1), rotation(1,0,0,0), localTransformDirty(true) {

}

ComponentTransform::~ComponentTransform() {

}

void ComponentTransform::updateLocalTransform() {
    if (!localTransformDirty)
        return;
    localTransformDirty = false;
    localTransform = (Eigen::Translation3f(position) * rotation.toRotationMatrix() * Eigen::Scaling(scale)).matrix();
}

void ComponentTransform::updateWorldTransform(ComponentTransform *parent) {
    TASSERT(!parent->localTransformDirty);
    TASSERT(!localTransformDirty);
    worldTransform = parent->worldTransform * localTransform;
}


