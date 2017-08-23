#pragma once
#include "TalonConfig.hpp"
#include "Eigen/Core"

TALON_NS_BEGIN

class ComponentCamera {
public:
    ComponentCamera();
    ~ComponentCamera();

    auto& getView() const {
        return view;
    }

    auto& getProjection() const {
        return projection;
    }

private:
    Eigen::Matrix4f view;
    Eigen::Matrix4f projection;

    void lookAt(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up);

    void setPerspective(float fovY, float aspect, float near, float far);
};



TALON_NS_END