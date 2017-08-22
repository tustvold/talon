#include "Scene.hpp"
#include "GameLoop.hpp"
#include "rendering/DeviceManager.hpp"
#include "rendering/WindowManager.hpp"
#include "system/GameSystemUpdateTransforms.hpp"
#include "system/RenderSystemMeshFilter.hpp"
#include "ApplicationServiceTable.hpp"
#include "rendering/SwapChain.hpp"

USING_TALON_NS;

Scene::Scene(int numRenderPasses) : numRenderPasses(numRenderPasses) {
    swapChain = std::make_unique<SwapChain>(numRenderPasses);
    gameLoop = std::make_unique<GameLoop>();

    addDefaultSystems();

    ApplicationServiceTable::windowManager->getWindowResizeEvent().Connect(this, &Scene::onWindowResized);
}

Scene::~Scene() {
    ApplicationServiceTable::windowManager->getWindowResizeEvent().Disconnect(this, &Scene::onWindowResized);
}



void Scene::addDefaultSystems() {
    gameLoop->addRenderSystem<RenderSystemMeshFilter>();

    gameLoop->addGameSystem<GameSystemUpdateTransforms>();
}

void Scene::recreateSwapChain() {
    ApplicationServiceTable::deviceManager->getDevice().waitIdle();
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
