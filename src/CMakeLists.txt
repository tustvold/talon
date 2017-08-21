
set(SOURCE_FILES ApplicationDelegate.hpp Application.hpp
        Application.cpp rendering/VulkanLoader.cpp rendering/VulkanLoader.hpp rendering/Vertex.hpp rendering/DeviceManager.cpp
        rendering/DeviceManager.hpp rendering/InstanceManager.cpp rendering/InstanceManager.hpp rendering/WindowManager.cpp rendering/WindowManager.hpp
        rendering/SurfaceManager.cpp rendering/SurfaceManager.hpp ApplicationInitSettings.hpp rendering/SwapChain.cpp rendering/SwapChain.hpp
        rendering/RenderPass.cpp rendering/RenderPass.hpp rendering/Material.cpp rendering/Material.hpp ServiceTable.hpp
        ServiceProvider.hpp ServiceTable.cpp ecs/TWorld.hpp ecs/ComponentStorageArray.hpp ecs/ComponentStorageMap.hpp
        ecs/ComponentStorage.hpp ecs/IdentifierPool.hpp Logging.hpp ${TALON_CONFIG_DIR}/TalonConfig.hpp rendering/MemoryAllocator.cpp rendering/MemoryAllocator.hpp ecs/View.hpp rendering/Mesh.cpp rendering/Mesh.hpp rendering/CommandPool.cpp rendering/CommandPool.hpp GameLoop.cpp GameLoop.hpp rendering/VSemaphore.hpp rendering/VSemaphore.cpp rendering/CommandBufferCollection.cpp rendering/CommandBufferCollection.hpp rendering/DebugCallback.cpp rendering/DebugCallback.hpp)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(talon ${SOURCE_FILES})
target_include_directories(talon PRIVATE ${GLBINDING_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR})
target_include_directories(talon PUBLIC ${INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${TALON_CONFIG_DIR})
target_link_libraries(talon PUBLIC tbb Vulkan::Vulkan ${GLFW_LIBRARIES} signals vulkan_mem_alloc)

