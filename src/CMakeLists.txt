
set(SOURCE_FILES Talon.hpp ApplicationDelegate.hpp Application.hpp
        Application.cpp VulkanLoader.cpp VulkanLoader.hpp Vertex.hpp DeviceManager.cpp
        DeviceManager.hpp InstanceManager.cpp InstanceManager.hpp WindowManager.cpp WindowManager.hpp
        SurfaceManager.cpp SurfaceManager.hpp ApplicationInitSettings.hpp SwapChain.cpp SwapChain.hpp RenderPass.cpp RenderPass.hpp Material.cpp Material.hpp DeviceProvider.hpp ServiceTable.hpp ServiceProvider.hpp ServiceTable.cpp ecs/World.hpp ecs/ComponentArrayStorage.hpp ecs/ComponentMapStorage.hpp ecs/ComponentStorage.hpp ecs/Entity.hpp ecs/IdentifierPool.hpp)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(talon ${SOURCE_FILES})
target_include_directories(talon PRIVATE ${GLBINDING_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR})
target_include_directories(talon PUBLIC ${INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(talon PUBLIC tbb Vulkan::Vulkan ${GLFW_LIBRARIES} signals)

