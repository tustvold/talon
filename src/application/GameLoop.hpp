#pragma once
#include <WorldConfig.hpp>
#include "TalonConfig.hpp"
#include "rendering/system/VSemaphore.hpp"
#include "rendering/system/CommandBuffer.hpp"
#include "ecs/system/RenderSystem.hpp"
#include "ecs/system/GameSystem.hpp"

TALON_NS_BEGIN

class SwapChain;
class RenderPass;
class Material;
class Mesh;
class DeviceManager;
class SurfaceManager;
class WindowManager;


class GameLoop {
public:
    GameLoop();
    ~GameLoop();
    bool doUpdate(World& world, SwapChain* swapChain);

    void addRenderSystem(std::unique_ptr<RenderSystem> &&renderSystem);
    void addGameSystem(std::unique_ptr<GameSystem> &&gameSystem);

    template<typename T>
    void addRenderSystem() {
        addRenderSystem(std::unique_ptr<RenderSystem>(new T()));
    }

    template<typename T>
    void addGameSystem() {
        addGameSystem(std::unique_ptr<GameSystem>(new T()));
    }

private:
    PrimaryCommandBuffer commandBuffer;

    std::vector<std::unique_ptr<RenderSystem>> renderSystems;
    std::vector<std::unique_ptr<GameSystem>> gameSystems;

    VSemaphore imageAvailableSemaphore;
    VSemaphore renderFinishedSemaphore;

    uint32_t updateCount;
    double updateStartInterval;

    void recordCommandBuffer(World& world, SwapChain* swapChain, int index);
};

TALON_NS_END