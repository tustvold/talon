#include "RenderSystemMeshFilter.hpp"
#include "rendering/system/RenderPass.hpp"

USING_TALON_NS;

void RenderSystemMeshFilter::update(const RenderSystemArgs& args) {
    args.world->for_each<ComponentTransform, ComponentMeshFilter>(boost::hana::fuse(
        [&args](EntityID id, boost::hana::tuple<const ComponentTransform *, const ComponentMeshFilter *> components) {
            //ComponentTransform* transform = components[0_c];
            const ComponentMeshFilter* meshFilter = components[1_c];

            args.renderPass->bindMaterial(meshFilter->material, args.commandBuffer);
            meshFilter->mesh->bind(args.commandBuffer);
            meshFilter->mesh->draw(args.commandBuffer);
        }));
}