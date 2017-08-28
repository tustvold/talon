#pragma once
#include "TalonConfig.hpp"
#include "TalonTypes.hpp"
#include <boost/container/flat_map.hpp>
#include <vulkan/vulkan.hpp>
#include <unordered_map>
#include "MaterialPipeline.hpp"

TALON_NS_BEGIN

class Material;
class CommandBuffer;
class SwapChain;
class RenderPass;

class MaterialPipelineCache {
public:
    MaterialPipelineCache();
    ~MaterialPipelineCache();

    void bindMaterial(Material* material, SwapChain* swapChain, RenderPass* renderPass, CommandBuffer* buffer);
private:
    std::unordered_map<MaterialID, std::unique_ptr<MaterialPipeline>> cache;
};



TALON_NS_END