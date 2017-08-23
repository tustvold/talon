#pragma once
#include <ecs/system/RenderSystem.hpp>
#include <ecs/system/GameSystem.hpp>
#include "TalonConfig.hpp"
#include "WorldConfig.hpp"


TALON_NS_BEGIN

class GameLoop;

class Scene {
public:
    World &getWorld() {
        return world;
    }

    void renderFrame();

    virtual ~Scene();

protected:
    explicit Scene(int numRenderPasses);

    const int numRenderPasses;

    World world;
    std::unique_ptr<GameLoop> gameLoop;
    std::unique_ptr<SwapChain> swapChain;

private:

    void addDefaultSystems();
    void onWindowResized(vk::Extent2D extent2D);
    void recreateSwapChain();
};

TALON_NS_END