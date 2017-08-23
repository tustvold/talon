
set(SOURCE_FILES application/Application.hpp
        application/Application.cpp rendering/singleton/VulkanLoader.cpp rendering/singleton/VulkanLoader.hpp rendering/mesh/Vertex.hpp rendering/singleton/DeviceManager.cpp
        rendering/singleton/DeviceManager.hpp rendering/singleton/InstanceManager.cpp rendering/singleton/InstanceManager.hpp rendering/singleton/WindowManager.cpp
        rendering/singleton/WindowManager.hpp rendering/singleton/SurfaceManager.cpp rendering/singleton/SurfaceManager.hpp application/ApplicationInitSettings.hpp rendering/system/SwapChain.cpp
        rendering/system/SwapChain.hpp rendering/system/RenderPass.cpp rendering/system/RenderPass.hpp rendering/material/Material.cpp rendering/material/Material.hpp
        rendering/singleton/RenderServiceTable.hpp util/ServiceProvider.hpp rendering/singleton/RenderServiceTable.cpp ecs/TWorld.hpp ecs/ComponentStorageArray.hpp ecs/ComponentStorageMap.hpp
        ecs/ComponentStorage.hpp ecs/IdentifierPool.hpp util/Logging.hpp ${TALON_CONFIG_DIR}/TalonConfig.hpp ${TALON_CONFIG_DIR}/WorldConfig.hpp rendering/singleton/MemoryAllocator.cpp
        rendering/singleton/MemoryAllocator.hpp ecs/View.hpp rendering/mesh/Mesh.cpp rendering/mesh/Mesh.hpp rendering/singleton/CommandPool.cpp rendering/singleton/CommandPool.hpp
        application/GameLoop.cpp application/GameLoop.hpp rendering/system/VSemaphore.hpp rendering/system/VSemaphore.cpp rendering/system/CommandBuffer.cpp
        rendering/system/CommandBuffer.hpp rendering/singleton/DebugCallback.cpp rendering/singleton/DebugCallback.hpp
        ecs/ComponentStorageTree.hpp ecs/component/ComponentTransform.hpp util/Util.hpp ecs/system/GameSystemUpdateTransforms.cpp
        ecs/system/GameSystemUpdateTransforms.hpp ecs/component/ComponentMeshFilter.hpp
        ecs/system/RenderSystemMeshFilter.cpp ecs/system/RenderSystemMeshFilter.hpp ecs/system/GameSystem.hpp
        ecs/system/RenderSystem.hpp application/Scene.cpp application/Scene.hpp rendering/material/MaterialPipeline.cpp rendering/material/MaterialPipeline.hpp
        ecs/system/RenderSystem.cpp rendering/material/MaterialPipelineCache.cpp rendering/material/MaterialPipelineCache.hpp ecs/component/ComponentCamera.cpp ecs/component/ComponentCamera.hpp util/Math.hpp ecs/component/ComponentTransform.cpp)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(talon ${SOURCE_FILES})
target_include_directories(talon PRIVATE ${GLBINDING_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR})
target_include_directories(talon PUBLIC ${INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${TALON_CONFIG_DIR} ${EIGEN3_INCLUDE_DIR})
target_link_libraries(talon PUBLIC Vulkan::Vulkan ${GLFW_LIBRARIES} signals vulkan_mem_alloc)

