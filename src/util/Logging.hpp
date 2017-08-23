#pragma once
#include "TalonConfig.hpp"
#include <stdexcept>

// Stringified version of __LINE__ macro

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

#define S_LOCATION __FILE__ ":" S__LINE__

// ANSI Color Codes

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Logging Macros

#define TLOGINFO(msg, ...) printf(ANSI_COLOR_BLUE "INFO: " msg " - " S_LOCATION ANSI_COLOR_RESET "\n", __VA_ARGS__)
#define TLOGWARN(msg, ...) printf(ANSI_COLOR_YELLOW "WARNING: " msg " - " S_LOCATION ANSI_COLOR_RESET "\n", __VA_ARGS__)
#define TLOGERROR(msg, ...) printf(ANSI_COLOR_RED "ERROR: " msg " - " S_LOCATION ANSI_COLOR_RESET "\n", __VA_ARGS__)

// Assertions

#define TASSERT(condition) TALON_NS::Assert(!!(condition), "Condition (" #condition ") failed at " S_LOCATION)

TALON_NS_BEGIN

static constexpr void Assert(bool condition, const std::string& msg) {
    if (!condition) {
        throw std::runtime_error(msg);
    }
}

TALON_NS_END