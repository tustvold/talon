#pragma once
#include "TalonConfig.hpp"
#include "RenderSystem.hpp"
#include <WorldConfig.hpp>

TALON_NS_BEGIN

class RenderSystemMeshFilter : public RenderSystem {
public:
    RenderSystemMeshFilter();
    ~RenderSystemMeshFilter() override;

    void update(const RenderSystemArgs& args) override;

private:
    SecondaryCommandBuffer commandBuffer;
};


TALON_NS_END