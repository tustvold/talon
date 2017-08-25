#include "GameSystemUpdateTransforms.hpp"

USING_TALON_NS;

void GameSystemUpdateTransforms::update(World &world) {
    auto &modelMatrixStorage = world.getComponentStorage<ComponentModelMatrix>();



    world.for_each<ComponentTransform, ComponentModelMatrix>(boost::hana::fuse([](
        EntityID id, boost::hana::tuple<ComponentTransform *, ComponentModelMatrix *> components) {
        ComponentTransform *a = components[0_c];
        if (!a->dirty)
            return;
        ComponentModelMatrix *b = components[1_c];
        a->updateTransform(b->modelMatrix);
    }));


    // This may not have the best possible performance
    // Options to improve it are
    // - change ComponentModelMatrix to be in a RandomAccess friendly storage container
    // - extend View to allow tree_for_each with additional components
    world.tree_for_each<ComponentTransformTree>(boost::hana::fuse([&modelMatrixStorage](EntityID id,
                                                                                        ComponentTransformTree *component,
                                                                                        EntityID parent_id,
                                                                                        ComponentTransformTree *parent) {

        Eigen::Matrix4f localTransform;
        component->updateTransform(localTransform);

        ComponentModelMatrix *modelMatrixComponent = modelMatrixStorage.get(id);
        ComponentModelMatrix *parentModelMatrixComponent = modelMatrixStorage.get(parent_id);
        modelMatrixComponent->modelMatrix = parentModelMatrixComponent->modelMatrix * localTransform;
    }));


}
