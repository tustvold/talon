#pragma once
#include "TalonConfig.hpp"
#include "SurfaceManager.hpp"
#include "DeviceManager.hpp"

TALON_NS_BEGIN

class SwapChain;
class RenderPass;
class Material;
class Mesh;

class RenderLoop {
public:
    RenderLoop(DeviceManager* deviceManger, SurfaceManager* surfaceManager, WindowManager* windowManager);
    ~RenderLoop();

    bool renderFrame(DeviceManager* deviceManger, SurfaceManager* surfaceManager);

private:
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<RenderPass> renderPass;
    std::unique_ptr<Material> material;

    std::vector<vk::CommandBuffer> commandBuffers;
    std::unique_ptr<Mesh> testMesh;

    vk::Semaphore imageAvailableSemaphore;
    vk::Semaphore renderFinishedSemaphore;

    uint32_t updateCount;
    double updateStartInterval;

    void recordCommandBuffer(int index);
};



TALON_NS_END