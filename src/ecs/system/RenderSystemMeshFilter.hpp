#pragma once
#include "TalonConfig.hpp"
#include "RenderSystem.hpp"
#include <WorldConfig.hpp>

TALON_NS_BEGIN

class RenderSystemMeshFilter : public RenderSystem {
public:
    void update(const RenderSystemArgs& args) override;
};


TALON_NS_END