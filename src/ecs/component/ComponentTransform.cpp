#include "ComponentTransform.hpp"

USING_TALON_NS;

ComponentTransform::ComponentTransform() : position(0, 0, 0), scale(1, 1, 1), rotation(1, 0, 0, 0), dirty(false) {

}

ComponentTransform::~ComponentTransform() = default;


void ComponentTransform::updateTransform(Eigen::Matrix4f &transform) const {
    transform = (Eigen::Translation3f(position) * rotation.toRotationMatrix() * Eigen::Scaling(scale)).matrix();
}