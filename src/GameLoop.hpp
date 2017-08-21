#pragma once
#include "TalonConfig.hpp"
#include "SurfaceManager.hpp"
#include "DeviceManager.hpp"
#include "VSemaphore.hpp"
#include "CommandBufferCollection.hpp"

TALON_NS_BEGIN

class SwapChain;
class RenderPass;
class Material;
class Mesh;

class GameLoop {
public:
    GameLoop(DeviceManager* deviceManger, SurfaceManager* surfaceManager, WindowManager* windowManager);
    bool renderFrame(DeviceManager* deviceManger, SurfaceManager* surfaceManager);

private:
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<RenderPass> renderPass;

    CommandBufferCollection commandBuffers;

    std::unique_ptr<Mesh> testMesh;
    std::unique_ptr<Material> testMaterial;

    VSemaphore imageAvailableSemaphore;
    VSemaphore renderFinishedSemaphore;

    uint32_t updateCount;
    double updateStartInterval;

    void recordCommandBuffer(int index);
};



TALON_NS_END