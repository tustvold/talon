#pragma once
#include "TalonConfig.hpp"
#include "TalonTypes.hpp"
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "MaterialPipeline.hpp"

TALON_NS_BEGIN

class Material;

class MaterialPipelineCache {
public:
    MaterialPipelineCache();
    ~MaterialPipelineCache();

    void bindMaterial(Material* material, SwapChain* swapChain, RenderPass* renderPass, vk::CommandBuffer buffer);
private:
    std::unordered_map<MaterialID, MaterialPipeline> cache;
};



TALON_NS_END