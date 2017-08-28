#include "MaterialPipelineCache.hpp"
#include "Material.hpp"
#include "MaterialPipeline.hpp"

USING_TALON_NS;

MaterialPipelineCache::MaterialPipelineCache() = default;
MaterialPipelineCache::~MaterialPipelineCache() = default;

void MaterialPipelineCache::bindMaterial(Material *material,
                                         SwapChain *swapChain,
                                         RenderPass *renderPass,
                                         CommandBuffer *buffer) {
    auto it = cache.find(material->getMaterialID());
    if (it != cache.end()) {
        it->second->bind(buffer);
        return;
    }

    auto ret = cache.emplace(std::piecewise_construct,
                             std::forward_as_tuple(material->getMaterialID()),
                             std::forward_as_tuple(material->createPipeline(swapChain, renderPass)));
    TASSERT(ret.second);
    ret.first->second->bind(buffer);
}
