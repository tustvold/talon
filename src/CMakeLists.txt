
set(SOURCE_FILES talon.h Greeter.cpp Greeter.h GameLoop.cpp GameLoop.h ApplicationDelegate.hpp Application.hpp Application.cpp vulkan_ext.cpp vulkan_ext.hpp Vertex.hpp)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(talon ${SOURCE_FILES})
target_include_directories(talon PRIVATE ${GLBINDING_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR})
target_include_directories(talon PUBLIC ${INCLUDE_DIR} ${FRUIT_INCLUDE_DIR} ${TBB_INCLUDE_DIR})
target_link_libraries(talon PUBLIC ${FRUIT_LIBRARIES} tbb Vulkan::Vulkan ${GLFW_LIBRARIES})

