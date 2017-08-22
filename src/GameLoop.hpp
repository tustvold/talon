#pragma once
#include <WorldConfig.hpp>
#include "TalonConfig.hpp"
#include "rendering/VSemaphore.hpp"
#include "rendering/CommandBufferCollection.hpp"

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
    GameLoop(DeviceManager* deviceManger, SurfaceManager* surfaceManager, WindowManager* windowManager);
    ~GameLoop();
    bool renderFrame(DeviceManager* deviceManger, SurfaceManager* surfaceManager);

private:
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<RenderPass> renderPass;

    CommandBufferCollection commandBuffers;

    std::unique_ptr<Mesh> testMesh;
    std::unique_ptr<Material> testMaterial;

    World world;

    VSemaphore imageAvailableSemaphore;
    VSemaphore renderFinishedSemaphore;

    uint32_t updateCount;
    double updateStartInterval;

    void recordCommandBuffer(int index);
};

TALON_NS_END