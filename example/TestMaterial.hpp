#pragma once
#include <TalonConfig.hpp>
#include <rendering/material/Material.hpp>


class TestMaterial : public TALON_NS::Material {
public:
    std::unique_ptr<talon::MaterialPipeline> createPipeline(talon::SwapChain *swapChain,
                                                            talon::RenderPass *renderPass) override;
};