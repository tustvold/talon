#include "TestScene.hpp"
#include "TestVertex.hpp"
#include "TestMaterial.hpp"

USING_TALON_NS;

const std::vector<TestVertex> test_vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

TestScene::TestScene() : Scene(1) {
    testMaterial = std::make_unique<TestMaterial>();

    auto meshData = makeMeshData(test_vertices);
    testMesh = std::make_unique<Mesh>(meshData);

    ComponentMeshFilter filter;
    filter.mesh = testMesh.get();
    filter.material = testMaterial.get();

    ComponentTransform transform;
    ComponentModelMatrix modelMatrix;
    world.createEntity(transform, modelMatrix, filter);
}

TestScene::~TestScene() {

}
