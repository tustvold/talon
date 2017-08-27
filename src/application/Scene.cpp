#include "Scene.hpp"
#include "GameLoop.hpp"
#include "rendering/singleton/DeviceManager.hpp"
#include "rendering/singleton/impl/VulkanWindowManager.hpp"
#include "ecs/system/GameSystemUpdateTransforms.hpp"
#include "ecs/system/RenderSystemMeshFilter.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/system/SwapChain.hpp"

USING_TALON_NS;

Scene::Scene(int numRenderPasses) : numRenderPasses(numRenderPasses) {
    gameLoop = std::make_unique<GameLoop>();

    addDefaultSystems();

}

Scene::~Scene() {

}

void Scene::addDefaultSystems() {
    gameLoop->addRenderSystem<RenderSystemMeshFilter>();

    gameLoop->addGameSystem<GameSystemUpdateTransforms>();
}

bool Scene::renderFrame(SwapChain* swapChain) {
    return gameLoop->doUpdate(world, swapChain);
}