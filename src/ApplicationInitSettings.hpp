#pragma once
#include "Talon.hpp"
#include <vector>

TALON_NS_BEGIN

struct ApplicationInitSettings {
    int windowWidth;
    int windowHeight;
    const char* windowName;

    bool validationLayersEnabled;
    std::vector<const char*> validationLayers;
    std::vector<const char*> deviceExtensions;
};

TALON_NS_END