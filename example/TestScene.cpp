#include "TestScene.hpp"
#include <rendering/mesh/Vertex.hpp>
#include "rendering/mesh/Mesh.hpp"
#include "rendering/material/Material.hpp"

USING_TALON_NS;

const std::vector<Vertex> test_vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

TestScene::TestScene() : Scene(1) {
    testMaterial = std::make_unique<Material>();

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
