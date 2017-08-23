#include "Scene.hpp"
#include "GameLoop.hpp"
#include "rendering/singleton/DeviceManager.hpp"
#include "rendering/singleton/WindowManager.hpp"
#include "ecs/system/GameSystemUpdateTransforms.hpp"
#include "ecs/system/RenderSystemMeshFilter.hpp"
#include "rendering/singleton/RenderServiceTable.hpp"
#include "rendering/system/SwapChain.hpp"

USING_TALON_NS;

Scene::Scene(int numRenderPasses) : numRenderPasses(numRenderPasses) {
    swapChain = std::make_unique<SwapChain>(numRenderPasses);
    gameLoop = std::make_unique<GameLoop>();

    addDefaultSystems();

    RenderServiceTable::windowManager->getWindowResizeEvent().Connect(this, &Scene::onWindowResized);
}

Scene::~Scene() {
    RenderServiceTable::windowManager->getWindowResizeEvent().Disconnect(this, &Scene::onWindowResized);
}

void Scene::addDefaultSystems() {
    gameLoop->addRenderSystem<RenderSystemMeshFilter>();

    gameLoop->addGameSystem<GameSystemUpdateTransforms>();
}

void Scene::recreateSwapChain() {
    RenderServiceTable::deviceManager->getDevice().waitIdle();
    swapChain.reset();
    swapChain = std::make_unique<SwapChain>(numRenderPasses);
}

void Scene::renderFrame() {
    if (!gameLoop->doUpdate(world, swapChain.get())) {
        recreateSwapChain();
    }
}

void Scene::onWindowResized(vk::Extent2D extent2D) {
    recreateSwapChain();
}
