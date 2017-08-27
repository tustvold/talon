#pragma once
#include "TalonConfig.hpp"
#include "TalonTypes.hpp"
#include <boost/container/flat_map.hpp>
#include <vulkan/vulkan.hpp>
#include "MaterialPipeline.hpp"

TALON_NS_BEGIN

class Material;
class CommandBuffer;

class MaterialPipelineCache {
public:
    MaterialPipelineCache();
    ~MaterialPipelineCache();

    void bindMaterial(Material* material, SwapChain* swapChain, RenderPass* renderPass, CommandBuffer* buffer);
private:
    boost::container::flat_map<MaterialID, MaterialPipeline> cache;
};



TALON_NS_END