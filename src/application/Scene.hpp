#pragma once
#include <ecs/system/RenderSystem.hpp>
#include <ecs/system/GameSystem.hpp>
#include "TalonConfig.hpp"
#include "WorldConfig.hpp"


TALON_NS_BEGIN

class GameLoop;

class Scene {
public:
    virtual ~Scene();

    bool renderFrame(SwapChain* swapChain);

    int getNumRenderPasses() {
        return numRenderPasses;
    }

protected:
    explicit Scene(int numRenderPasses);

    const int numRenderPasses;

    World world;
    std::unique_ptr<GameLoop> gameLoop;

private:

    void addDefaultSystems();
};

TALON_NS_END