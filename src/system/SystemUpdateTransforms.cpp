#include "SystemUpdateTransforms.hpp"

USING_TALON_NS;

void SystemUpdateTransforms::update(World &world) {
    ComponentStorage<ComponentTransform> &transforms = world.getComponentStorage<ComponentTransform>();

    transforms.tree_for_each(boost::hana::fuse([](EntityID id,
                                                  ComponentTransform *component,
                                                  EntityID parent_id,
                                                  ComponentTransform *parent) {
        component->updateLocalTransform();
        if (parent)
            component->updateWorldTransform(parent);
        else
            component->worldTransform = component->localTransform;
    }));
}
