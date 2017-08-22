#include "SystemRenderMeshFilter.hpp"

void talon::SystemRenderMeshFilter::update(World& world, vk::CommandBuffer commandBuffer) {
    world.for_each<ComponentTransform, ComponentMeshFilter>(boost::hana::fuse(
        [commandBuffer](EntityID id, boost::hana::tuple<ComponentTransform *, ComponentMeshFilter *> components) {
            //ComponentTransform* transform = components[0_c];
            ComponentMeshFilter* meshFilter = components[1_c];

            meshFilter->material->bind(commandBuffer);
            meshFilter->mesh->bind(commandBuffer);
            meshFilter->mesh->draw(commandBuffer);
        }));
}
