
set(SOURCE_FILES talon.hpp ApplicationDelegate.hpp Application.hpp
        Application.cpp vulkan_ext.cpp vulkan_ext.hpp Vertex.hpp Shader.cpp Shader.hpp DeviceManager.cpp
        DeviceManager.hpp InstanceManager.cpp InstanceManager.hpp WindowManager.cpp WindowManager.hpp
        SurfaceManager.cpp SurfaceManager.hpp ApplicationInitSettings.hpp SwapChain.cpp SwapChain.hpp RenderPass.cpp RenderPass.hpp Material.cpp Material.hpp)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(talon ${SOURCE_FILES})
target_include_directories(talon PRIVATE ${GLBINDING_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR})
target_include_directories(talon PUBLIC ${INCLUDE_DIR} ${FRUIT_INCLUDE_DIR} ${TBB_INCLUDE_DIR})
target_link_libraries(talon PUBLIC tbb Vulkan::Vulkan ${GLFW_LIBRARIES} signals)
