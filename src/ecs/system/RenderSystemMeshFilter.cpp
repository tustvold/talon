#include "RenderSystemMeshFilter.hpp"
#include "rendering/system/RenderPass.hpp"

USING_TALON_NS;

RenderSystemMeshFilter::RenderSystemMeshFilter() = default;
RenderSystemMeshFilter::~RenderSystemMeshFilter() = default;

void RenderSystemMeshFilter::update(const RenderSystemArgs &args) {
    commandBuffer.reset();

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags |= vk::CommandBufferUsageFlagBits::eRenderPassContinue;
    beginInfo.setPInheritanceInfo(args.commandBufferInheritanceInfo);

    {
        auto recordHandle = commandBuffer.begin(beginInfo);

        args.world->for_each<ComponentModelMatrix, ComponentMeshFilter>(boost::hana::fuse(
            [this, &args](EntityID id, boost::hana::tuple<const ComponentModelMatrix *, const ComponentMeshFilter *> components) {
                //ComponentTransformTree* transform = components[0_c];
                const ComponentMeshFilter *meshFilter = components[1_c];

                args.renderPass->bindMaterial(args.swapChain, meshFilter->material, &commandBuffer);
                meshFilter->mesh->bind(&commandBuffer);
                meshFilter->mesh->draw(&commandBuffer);
            }));
    }

    args.primaryCommandBuffer->executeCommandBuffer(&commandBuffer);
}


