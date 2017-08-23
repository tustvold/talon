#pragma once
#include "TalonConfig.hpp"

TALON_NS_BEGIN

namespace math {
    static constexpr float pi = 3.14159265359f;

    static constexpr auto degToRad(float degrees) {
        return degrees / 180.0f * pi;
    }

    static constexpr auto radToDeg(float radians) {
        return radians / pi * 180.0f;
    }
}

TALON_NS_END