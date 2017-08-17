#pragma once

#include <stdexcept>

#define TALON_NS
#define TALON_NS_BEGIN namespace TALON_NS {

#define TALON_NS_END }

// Stringified version of __LINE__ macro
#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

#define S_LOCATION __FILE__ ":" S__LINE__

#define TASSERT(condition) TALON_NS::Assert(!!(condition), "Condition (" #condition ") failed at " S_LOCATION)

TALON_NS_BEGIN

static constexpr void Assert(bool condition, const std::string& msg) {
    if (!condition) {
        throw std::runtime_error(msg);
    }
}

TALON_NS_END