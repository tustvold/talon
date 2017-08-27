#include <rendering/singleton/RenderServiceTable.hpp>
#include "ComponentCamera.hpp"
#include "Eigen/Geometry"
#include "util/Math.hpp"
#include "rendering/singleton/impl/VulkanWindowManager.hpp"

USING_TALON_NS;

ComponentCamera::ComponentCamera() : view(Eigen::Matrix4f::Zero()), projection(Eigen::Matrix4f::Zero()) {

    auto extents = RenderServiceTable::windowManager->getWindowExtents();

    lookAt(Eigen::Vector3f(2,2,2), Eigen::Vector3f(0,0,0), Eigen::Vector3f(0,0,1));
    setPerspective(math::degToRad(45), extents.width / (float)extents.height,  0.1f, 10.0f);
}

ComponentCamera::~ComponentCamera() {

}

void ComponentCamera::lookAt(const Eigen::Vector3f &position, const Eigen::Vector3f &target, const Eigen::Vector3f &up) {
    Eigen::Matrix3f R;
    R.col(2) = (position-target).normalized();
    R.col(0) = up.cross(R.col(2)).normalized();
    R.col(1) = R.col(2).cross(R.col(0));
    view.topLeftCorner<3,3>() = R.transpose();
    view.topRightCorner<3,1>() = -R.transpose() * position;
    view(3,3) = 1.0f;
}


void ComponentCamera::setPerspective(float fovY, float aspect, float near, float far) {
    float theta = fovY*0.5f;
    float range = far - near;
    float invtan = 1.f/tanf(theta);

    projection(0,0) = invtan / aspect;
    projection(1,1) = invtan;
    projection(2,2) = -(near + far) / range;
    projection(3,2) = -1;
    projection(2,3) = -2 * near * far / range;
    projection(3,3) = 0;
}
