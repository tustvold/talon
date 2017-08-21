
set(SOURCE_FILES ApplicationDelegate.hpp Application.hpp
        Application.cpp VulkanLoader.cpp VulkanLoader.hpp Vertex.hpp DeviceManager.cpp
        DeviceManager.hpp InstanceManager.cpp InstanceManager.hpp WindowManager.cpp WindowManager.hpp
        SurfaceManager.cpp SurfaceManager.hpp ApplicationInitSettings.hpp SwapChain.cpp SwapChain.hpp
        RenderPass.cpp RenderPass.hpp Material.cpp Material.hpp ServiceTable.hpp
        ServiceProvider.hpp ServiceTable.cpp ecs/TWorld.hpp ecs/ComponentStorageArray.hpp ecs/ComponentStorageMap.hpp
        ecs/ComponentStorage.hpp ecs/IdentifierPool.hpp Logging.hpp ${TALON_CONFIG_DIR}/TalonConfig.hpp MemoryAllocator.cpp MemoryAllocator.hpp ecs/View.hpp Mesh.cpp Mesh.hpp CommandPool.cpp CommandPool.hpp GameLoop.cpp GameLoop.hpp VSemaphore.hpp VSemaphore.cpp CommandBufferCollection.cpp CommandBufferCollection.hpp Util.cpp Util.hpp DebugCallback.cpp DebugCallback.hpp)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(talon ${SOURCE_FILES})
target_include_directories(talon PRIVATE ${GLBINDING_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR})
target_include_directories(talon PUBLIC ${INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${TALON_CONFIG_DIR})
target_link_libraries(talon PUBLIC tbb Vulkan::Vulkan ${GLFW_LIBRARIES} signals vulkan_mem_alloc)

